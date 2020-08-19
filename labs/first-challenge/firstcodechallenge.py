lst = [1, [2, [3, [4, [5, 6]]]]]


def get_length(list):
    return 1 if type(list) is int else sum(get_length(i) for i in list)


print(get_length(lst))
