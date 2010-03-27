def online_variance(data):
    n = 0
    mean = 0
    M2 = 0
 
    for x in data:
        n = n + 1
        delta = x - mean
        mean = mean + delta/n
        M2 = M2 + delta*(x - mean)  # This expression uses the new value of mean
 
    variance_n = M2/n
    variance = M2/(n - 1)
    return variance
