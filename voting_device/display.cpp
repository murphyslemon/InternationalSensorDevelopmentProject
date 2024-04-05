#include "display.h"
#include "images.h"

Epd epd;
unsigned char image[5*1024];
Paint paint(image, 0, 0);

void initDisplay(){
   //E-paper init
  epd.LDirInit();
  epd.Clear(); // clears whole screen to white
  paint.SetRotate(ROTATE_270); //rotate screen 
}

void startupScreen(int batteryLevel){
  //wifilogo
    paint.SetWidth(35);
    paint.SetHeight(35);
    paint.Clear(UNCOLORED); // paints the height and width with the given color
    drawImage(0, 0, 35, 35, wifilogo); // draw the image at (0, 0) coordinates
    epd.SetFrameMemory(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
  //battery logo
    paint.Clear(UNCOLORED);
    drawImage(0, 0, 35, 35, batterylogo); // draw the image at (0, 0) coordinates
  //battery status bar
    paint.DrawFilledRectangle(4, 8, batteryLevel, 24, COLORED);
    epd.SetFrameMemory(paint.GetImage(), 0, (200-35-3), paint.GetWidth(), paint.GetHeight());
  //Welcome msg
    char text[9] = "WELCOME!";
    paint.SetWidth(30);
    paint.SetHeight(200);
    paint.Clear(UNCOLORED); // paints the height and width with the given color
    paint.DrawStringAt((200-strlen(text)*17)/2, 0, text, &Font24, COLORED); //moves text to co-ordinates with-in the set height and width
    paint.DrawStringAt((200-strlen(text)*17)/2+1, 0, text, &Font24, COLORED);
    epd.SetFrameMemory(paint.GetImage(), 100, 0, paint.GetWidth(), paint.GetHeight()); //moves page to co-ordinates
    epd.DisplayFrame();
}

static void clearQuestionArea(){
  int offset = 36;
  for (int i = 0; i < 3; i++){
    paint.SetWidth(40);
    paint.SetHeight(200);
    paint.Clear(UNCOLORED);
    epd.SetFrameMemory(paint.GetImage(), offset, 0, paint.GetWidth(), paint.GetHeight());
    offset += 40;
  }  
  paint.SetWidth(23);
  paint.SetHeight(200);
  paint.Clear(UNCOLORED);
  epd.SetFrameMemory(paint.GetImage(), offset, 0, paint.GetWidth(), paint.GetHeight());
}

void paintVoteScreen(const char *question) { //battery level is a range between 4 and 28, ie. battery at 100% = 28, battery empty=4
  //clear screen
    clearQuestionArea();
    //Bottom buttton bar
    paint.SetWidth(22);
    paint.SetHeight(200);
    paint.Clear(UNCOLORED); //paints the height and width the given colour
    paint.DrawStringAt(2, 6, "YES ABSTAIN NO", &Font20, COLORED); //moves text to co-ordinates with-in the set height and width
    paint.DrawFilledRectangle(0, 0, 200, 3, COLORED);
    paint.DrawFilledRectangle(48, 0, 50, 25, COLORED);
    paint.DrawFilledRectangle(160, 0, 162, 25, COLORED);
    epd.SetFrameMemory(paint.GetImage(), (200-22), 0, paint.GetWidth(), paint.GetHeight()); //moves page to co-ordinates
  //Question
    int position = 0;
    char line[16];
    size_t sizeOfString = strlen(question);  // Size of the string
    if(sizeOfString <= 90){
      display_question(question, line, &position);
    }
    else {
      char stringMod[90]; // Including space for null terminator
      strncpy(stringMod, question, 90);
      stringMod[89] = '\0';
      display_question(stringMod, line, &position); // Pass the modified string to display_question
    }
    epd.DisplayFrame();
}

void paintConfirmScreen(const char *response) {
  //clear screen
    clearQuestionArea();
  //Bottom button bar
    paint.SetWidth(22);
    paint.SetHeight(200);
    paint.Clear(UNCOLORED); //paints the height and width the given colour
    paint.DrawStringAt(2, 6, "YES         NO", &Font20, COLORED); //moves text to co-ordinates with-in the set height and width
    paint.DrawFilledRectangle(0, 0, 200, 3, COLORED);
    paint.DrawFilledRectangle(48, 0, 50, 25, COLORED);
    paint.DrawFilledRectangle(160, 0, 162, 25, COLORED);
    epd.SetFrameMemory(paint.GetImage(), (200-22), 0, paint.GetWidth(), paint.GetHeight()); //moves page to co-ordinates
  //Confirmation question (Are you sure?)
    paint.SetWidth(26);
    paint.SetHeight(200);
    paint.Clear(UNCOLORED); //paints the height and width the given colour
    paint.DrawStringAt(20, 0, "You voted:", &Font20, COLORED); //moves text to co-ordinates with-in the set height and width
    epd.SetFrameMemory(paint.GetImage(), 40, 0, paint.GetWidth(), paint.GetHeight());
    paint.Clear(UNCOLORED); //paints the height and width the given colour
    paint.DrawStringAt((200-strlen(response)*17)/2, 0, response, &Font24, COLORED); //moves text to co-ordinates with-in the set height and width
    paint.DrawStringAt((200-strlen(response)*17)/2+1, 0, response, &Font24, COLORED); //moves text to co-ordinates with-in the set height and width
    epd.SetFrameMemory(paint.GetImage(), 80, 0, paint.GetWidth(), paint.GetHeight());
    paint.Clear(UNCOLORED); //paints the height and width the given colour
    paint.DrawStringAt(10, 0, "Are you sure?", &Font20, COLORED); //moves text to co-ordinates with-in the set height and width
    epd.SetFrameMemory(paint.GetImage(), 140, 0, paint.GetWidth(), paint.GetHeight());

    epd.DisplayFrame();
}

void paintClosingScreen() {
    paint.SetWidth(200);
    paint.SetHeight(200);
    paint.Clear(UNCOLORED);
    epd.SetFrameMemory(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight()); //moves page to co-ordinates
    drawImage(0,0,200,180,thankyoulogo);
    epd.SetFrameMemory(paint.GetImage(), 10, 0, paint.GetWidth(), paint.GetHeight()); //moves page to co-ordinates
    epd.DisplayFrame();
}

//helper functions
void drawImage(int x, int y, int width, int height, const unsigned char *image) {
    int byteWidth = (width + 7) / 8; // Number of bytes in a row
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            if (pgm_read_byte(image + j * byteWidth + i / 8) & (128 >> (i % 8))) {
                paint.DrawPixel(x + i, y + j, COLORED);
            } else {
                paint.DrawPixel(x + i, y + j, UNCOLORED);
            }
        }
    }
}

void display_question(const char *question, char *line, int *position) {
    size_t sizeOfString = strlen(question);
    paint.SetWidth(16);
    paint.SetHeight(200);
    int count = 0;
    do {
        strncpy(line, question + *position, 15);
        line[15] = '\0';

        int pos = 15;
        int extra = 0;
        while (pos >= 0 && line[pos] != ' ') {
            pos--;
            extra++;
        }
        line[pos] = '\0';
        
        if (extra == 16) {
            line[14] = '-';
            *position -= 1;  // Update position to after the '-'
        } else {
            *position -= extra;  // Update position to the last space
            *position += 1;  // Update position to after the space
        }
        paint.Clear(UNCOLORED); //paints the height and width the given colour
        paint.DrawStringAt(20, 0, line, &Font16, COLORED); //moves text to co-ordinates with-in the set height and width
        epd.SetFrameMemory(paint.GetImage(), (40+(count*16)),0, paint.GetWidth(), paint.GetHeight()); //moves page to co-ordinate
        *position += 15;
        count++;
    } while ((*position) < sizeOfString);
}
