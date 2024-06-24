#include <Wire.h>
#include <Adafruit_AMG88xx.h>

Adafruit_AMG88xx amg;
float pixels[AMG88xx_PIXEL_ARRAY_SIZE];
// Use a dynamically allocated array to save static memory, allocate only when needed
float *interpolatedPixels = NULL; // A pointer for a 16x16 grid

void setup() {
  Serial.begin(115200);
  amg.begin();

  // Dynamically allocate memory for the 16x16 interpolated grid
  interpolatedPixels = new float[16 * 16];
  if (!interpolatedPixels) {
    Serial.println("Failed to allocate memory for interpolatedPixels.");
    while (1); // Halt if memory allocation failed
  }
}

void interpolatePixels() {
    for (int y = 0; y < 16; y++) {
        for (int x = 0; x < 16; x++) {
            int origX = x / 2;
            int origY = y / 2;

            float topLeft = pixels[origY * 8 + origX];
            float topRight = pixels[origY * 8 + min(origX + 1, 7)];
            float bottomLeft = pixels[min(origY + 1, 7) * 8 + origX];
            float bottomRight = pixels[min(origY + 1, 7) * 8 + min(origX + 1, 7)];

            float tx = (x % 2) / 2.0;
            float ty = (y % 2) / 2.0;

            float topInterpol = topLeft * (1 - tx) + topRight * tx;
            float bottomInterpol = bottomLeft * (1 - tx) + bottomRight * tx;
            float finalInterpol = topInterpol * (1 - ty) + bottomInterpol * ty;

            interpolatedPixels[y * 16 + x] = finalInterpol;
        }
    }
}

void loop() {
  amg.readPixels(pixels);
  interpolatePixels();
  
  // Send the interpolated data
  for (int i = 0; i < 16 * 16; i++) {
    Serial.print(interpolatedPixels[i]);
    if (i < 16 * 16 - 1) Serial.print(",");
  }
  Serial.println();
  
  delay(100);
}

void freeMemory() {
  if (interpolatedPixels) {
    delete[] interpolatedPixels;
    interpolatedPixels = NULL;
  }
}

void haltOnError() {
  freeMemory();
  Serial.println("Error occurred. Halting.");
  while (1); // Halt indefinitely
}
