def calculate_min_max(data):
    if len(data) == 0:
        return None, None  # Handle empty array case
    
    # Initialize min and max with the first element
    min_value = data[0]
    max_value = data[0]
    
    # Iterate through the array to find min and max
    for num in data:
        if num < min_value:
            min_value = num
        if num > max_value:
            max_value = num
            
    return min_value, max_value

def calculate_mean(data):
    return sum(data) / len(data)

def calculate_variance(data, mean):
    return sum((x - mean) ** 2 for x in data) / len(data)

def calculate_standard_deviation(data):
    if len(data) == 0:
        return 0  # Handle empty array case

    mean = calculate_mean(data)
    variance = calculate_variance(data, mean)
    std_deviation = variance ** 0.5  # Square root of variance
    
    return std_deviation

def calculate_mean(arr):
    if not arr:  # Check if the array is empty
        return 0
    return sum(arr) / len(arr)

data = [
    155898, 155968, 156220, 156250, 156296, 156088, 155884,156192, 155850, 155844
]
mean_value = calculate_mean(data)
print(f"The mean of the array is: {mean_value}")
std_dev = calculate_standard_deviation(data)
print(f"The standard deviation of the data is: {std_dev:.5f}")
min_val, max_val = calculate_min_max(data)
print(f"The minimum value is: {min_val}")
print(f"The maximum value is: {max_val}")

