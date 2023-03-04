import cv2
import matplotlib.pyplot as plt
import numpy as np
import pydicom

x = np.arange(0, 5, 0.1)
y = np.sin(x)
plt.plot(x, y)

ds = pydicom.dcmread('assets/PE_Image.dcm')
print(ds)

image_2d = ds.pixel_array.astype(float)
print(image_2d)

image_2d_scaled = (np.maximum(image_2d, 0) / image_2d.max()) * 255.0
image_2d_scaled = np.uint8(image_2d_scaled)

# Creating GUI window to display an image on screen
# first Parameter is windows title (should be in string format)
# Second Parameter is image array
cv2.imshow("image", image_2d_scaled)
 
# To hold the window on screen, we use cv2.waitKey method
# Once it detected the close input, it will release the control
# To the next line
# First Parameter is for holding screen for specified milliseconds
# It should be positive integer. If 0 pass an parameter, then it will
# hold the screen until user close it.
cv2.waitKey(0)
 
# It is for removing/deleting created GUI window from screen
# and memory
cv2.destroyAllWindows()