// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 254.
//!+

// Chat is a server that lets clients chat with each other.
package main

import (
	"bufio"
	"flag"
	"fmt"
	"log"
	"net"
	"strings"
	"time"
)

//!+broadcaster
type client chan<- string // an outgoing message channel

var (
	entering    = make(chan client)
	leaving     = make(chan client)
	messages    = make(chan string) // all incoming client messages
	serverMsg   = "irc-server > "
	users       = make(map[string]string)
	userClients = make(map[string]client)
	ips         = make(map[string]string)
	connections = make(map[string]net.Conn)
	times       = make(map[string]string)
	admin       = ""
)

func broadcaster() {
	clients := make(map[client]bool) // all connected clients
	for {
		select {
		case msg := <-messages:
			// Broadcast incoming message to all
			// clients' outgoing message channels.
			for cli := range clients {
				cli <- msg
			}

		case cli := <-entering:
			clients[cli] = true

		case cli := <-leaving:
			delete(clients, cli)
			close(cli)
		}
	}
}

//!-broadcaster

//!+handleConn
func handleConn(conn net.Conn) {
	ch := make(chan string) // outgoing client messages
	go clientWriter(conn, ch)

	input := bufio.NewScanner(conn)
	input.Scan()
	who := input.Text()

	//Verification for the username and validation
	if users[who] == who {
		ch <- serverMsg + "Username already taken"
		close(ch)
		conn.Close()
		return
	}

	//If the user doesn´t exist it gets saved and created
	if users[who] == "" {
		users[who] = who
		userClients[who] = ch
		ips[who] = conn.RemoteAddr().String()
		connections[who] = conn

		hourAt, err := time.LoadLocation("America/Mexico_City")
		if err != nil {
			panic(err)
		}
		t := time.Now().In(hourAt)

		times[who] = t.Format("01-02-2006 15:04:05")
		ch <- "irc-server > Welcome to the Simple IRC Server"
		ch <- "irc-server > Your user [" + who + "] is successfully logged"
	}

	//In here the first client connected gets to be the admin
	if len(users) == 1 {
		admin = who
		ch <- "irc-server > Congrats, you were the first user."
		ch <- "irc-server > You're the new IRC Server ADMIN"
		fmt.Println("irc-server > [" + who + "] was promoted as the channel ADMIN")
	}

	//Notifications for new client connected
	messages <- serverMsg + "New connected user [" + who + "]"
	entering <- ch
	fmt.Println(serverMsg + "New connected user [" + who + "]")

	//Validations for commands sent by each user
	for input.Scan() {
		command := strings.Split(input.Text(), " ")
		switch command[0] {
		case "/users":
			for user := range users {
				ch <- serverMsg + user + " - connected since " + times[user]
			}
		case "/msg":
			if len(command) > 2 {
				if users[command[1]] == command[1] {
					userClients[command[1]] <- users[who] + " > " + strings.Join(command[2:], " ")
				}
				if users[command[1]] != command[1] {
					ch <- serverMsg + "User not found"
				}
				continue
			}
			ch <- serverMsg + "Incorrect use of command. Usage: /msg <username> <msg>"
		case "/time":
			hourAt, err := time.LoadLocation("America/Mexico_City")
			if err != nil {
				panic(err)
			}
			t := time.Now().In(hourAt)
			ch <- serverMsg + "Local Time: America/Mexico_City " + t.Format("15:04")
		case "/user":
			if len(command) == 2 {
				if users[command[1]] == command[1] {
					ch <- serverMsg + "username: " + users[command[1]] + ", IP: " + ips[command[1]] + " Connected since: " + times[command[1]]
				} else {
					ch <- serverMsg + "User doesn´t exist"
				}
				continue
			}
			ch <- serverMsg + "Incorrect use of command. Usage: /user <username>"
		case "/kick":
			if len(command) == 2 {
				if users[who] == admin {
					userClients[command[1]] <- "irc-server > You're kicked from this channel"
					userClients[command[1]] <- "irc-server > Bad language is not allowed on this channel"
					delete(users, command[1])
					delete(userClients, command[1])
					delete(ips, command[1])
					connections[command[1]].Close()
					delete(connections, command[1])
					delete(times, command[1])
					messages <- serverMsg + "[" + command[1] + "] was kicked from channel for bad language policy violation"
					fmt.Println(serverMsg + "[" + command[1] + "] was kicked")
					continue
				}
				ch <- serverMsg + "You are not the admin of the chat"
				continue
			}
			ch <- serverMsg + "Incorrect use of command. Usage: /kick <username>"
		default:
			for msg := range users {
				if msg != who {
					userClients[msg] <- who + " > " + input.Text()
				}

			}
		}

	}
	// NOTE: ignoring potential errors from input.Err()

	//Notification and elimination of users that left the chat
	leaving <- ch
	messages <- serverMsg + "[" + who + "] left channel"
	fmt.Println(serverMsg + "[" + who + "] left")
	delete(users, who)
	delete(userClients, who)
	delete(ips, who)
	delete(connections, who)
	delete(times, who)
	conn.Close()

	//Setting the new admin if the actual admin leaves the chat
	if admin == who {
		for user := range users {
			admin = user
			userClients[user] <- "irc-server > You're the new IRC Server ADMIN"
			fmt.Println("irc-server > [" + user + "] was promoted as the channel ADMIN")
			break
		}
	}
}

func clientWriter(conn net.Conn, ch <-chan string) {
	for msg := range ch {
		fmt.Fprintln(conn, msg) // NOTE: ignoring network errors
	}
}

//!-handleConn

//!+main
func main() {

	dir := flag.String("host", "localhost", "Default to localhost.")
	port := flag.String("port", "9000", "Default to 9000")
	flag.Parse()

	listener, err := net.Listen("tcp", *dir+":"+*port)
	if err != nil {
		log.Fatal(err)
	}

	fmt.Printf("%sSimple IRC Server started at %s:%s\n", serverMsg, *dir, *port)
	fmt.Printf("%sReady for receiving new clients\n", serverMsg)

	go broadcaster()
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err)
			continue
		}
		go handleConn(conn)
	}
}

//!-main
