#include <ADXL335.h>

#include <SoftwareSerial.h>

#include <LedControl.h>

#include <math.h>

ADXL335 accelero;

struct Point
{
  int x;
  int y;
};
typedef Point Direction;

struct Buffer
{
  Buffer(){}
  Buffer(bool n_contents[8][8])
  {
    for (int x = 0; x != 8; x++)
    {
      for (int y = 0; y != 8; y++)
      {
        contents[x][y] = n_contents[x][y];
      }
    }
  }
  bool contents[8][8];
};

/*
This will move the contents of the given buffer right by 1 pixel.
*/
int shiftBufferToRight(Buffer* buffer_to_shift)
{
  Buffer temp;

  for (int x = 0; x != 8; x++)
  {
    for (int y = 0; y != 8; y++)
    {
      temp.contents[x][y] = buffer_to_shift->contents[x - 1][y];
    }
  }

  for (int y = 0; y != 8; y++)
  {
    temp.contents[0][y] = false;
  }

  copyBuffer(buffer_to_shift, &temp);
}
void shiftBufferToRightWithWrap(Buffer* buffer_to_shift)
{
  Buffer temp;

  for (int x = 0; x != 8; x++)
  {
    for (int y = 0; y != 8; y++)
    {
      temp.contents[x][y] = buffer_to_shift->contents[x - 1][y];
    }
  }

  for (int y = 0; y != 8; y++)
  {
    temp.contents[0][y] = buffer_to_shift->contents[7][y];
  }

  copyBuffer(buffer_to_shift, &temp);
}
/*
This will move the contents of the given buffer left by 1 pixel.
*/
void shiftBufferToLeft(Buffer* buffer_to_shift)
{
  Buffer temp;

  for (int x = 0; x != 8; x++)
  {
    for (int y = 0; y != 8; y++)
    {
      temp.contents[x][y] = buffer_to_shift->contents[x + 1][y];
    }
  }

  for (int y = 0; y != 8; y++)
  {
    temp.contents[7][y] = false;
  }

  copyBuffer(buffer_to_shift, &temp);
}
void shiftBufferToLeftWithWrap(Buffer* buffer_to_shift)
{
  Buffer temp;

  for (int x = 0; x != 8; x++)
  {
    for (int y = 0; y != 8; y++)
    {
      temp.contents[x][y] = buffer_to_shift->contents[x + 1][y];
    }
  }

  for (int y = 0; y != 8; y++)
  {
    temp.contents[7][y] = buffer_to_shift->contents[0][y];
  }

  copyBuffer(buffer_to_shift, &temp);
}
/*
This will move the contents of the given buffer up by 1 pixel.
*/
void shiftBufferUp(Buffer* buffer_to_shift)
{
  Buffer temp;

  for (int x = 0; x != 8; x++)
  {
    for (int y = 0; y != 8; y++)
    {
      temp.contents[x][y] = buffer_to_shift->contents[x][y - 1];
    }
  }

  for (int x = 0; x != 8; x++)
  {
    temp.contents[x][0] = false;
  }

  copyBuffer(buffer_to_shift, &temp);
}
void shiftBufferUpWithWrap(Buffer* buffer_to_shift)
{
  Buffer temp;

  for (int x = 0; x != 8; x++)
  {
    for (int y = 0; y != 8; y++)
    {
      temp.contents[x][y] = buffer_to_shift->contents[x][y - 1];
    }
  }

  for (int x = 0; x != 8; x++)
  {
    temp.contents[x][0] = buffer_to_shift->contents[x][7];
  }

  copyBuffer(buffer_to_shift, &temp);
}
/*
This will move the contents of the given buffer down by 1 pixel.
*/
void shiftBufferDown(Buffer* buffer_to_shift)
{
  Buffer temp;

  for (int x = 0; x != 8; x++)
  {
    for (int y = 0; y != 8; y++)
    {
      temp.contents[x][y] = buffer_to_shift->contents[x][y + 1];
    }
  }

  for (int x = 0; x != 8; x++)
  {
    temp.contents[x][7] = false;
  }

  copyBuffer(buffer_to_shift, &temp);
}
void shiftBufferDownWithWrap(Buffer* buffer_to_shift)
{
  Buffer temp;

  for (int x = 0; x != 8; x++)
  {
    for (int y = 0; y != 8; y++)
    {
      temp.contents[x][y] = buffer_to_shift->contents[x][y + 1];
    }
  }

  for (int x = 0; x != 8; x++)
  {
    temp.contents[x][7] = buffer_to_shift->contents[x][0];
  }

  copyBuffer(buffer_to_shift, &temp);
}

void drawHeart(Buffer* buffer_to_draw_to)
{
  bool heart[8][8] = {
    {0,0,0,0,0,0,0,0},
    {0,0,1,0,0,1,0,0},
    {0,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,0},
    {0,0,1,1,1,1,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,0,0,0,0,0},
  };

  Buffer temp(heart);

  copyBuffer(buffer_to_draw_to, &temp);
  flipBufferVertically(buffer_to_draw_to);
}
void drawCircle(Buffer* buffer_to_draw_to)
{
  bool circle[8][8] = {
  {0,0,0,0,0,0,0,0},
  {0,0,0,1,1,1,0,0},
  {0,0,1,1,1,1,1,0},
  {0,0,1,1,1,1,1,0},
  {0,0,1,1,1,1,1,0},
  {0,0,0,1,1,1,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  };

  Buffer temp(circle);

  copyBuffer(buffer_to_draw_to, &temp);
  flipBufferVertically(buffer_to_draw_to);
}

int draw(LedControl* p_LC, int x, int y)
{
  p_LC->setLed(0, x, y, true);
}

int clearBuffer(Buffer* buffer_to_clear)
{
  for (int x = 0; x != 8; x++)
  {
    for (int y = 0; y != 8; y++)
    {
      buffer_to_clear->contents[x][y] = false;
    }
  }
}

int drawScreenBuffer(LedControl* p_LC, Buffer* buffer_to_draw)
{
  p_LC->clearDisplay(0);

  for (int x = 0; x != 8; x++)
  {
    for (int y = 0; y != 8; y++)
    {
      if (buffer_to_draw->contents[x][y])
      {
        draw(p_LC, x, y);
      }
    }
  }
}

int mapPointIntoBuffer(Point* p_point, Buffer* target_buffer)
{
  target_buffer->contents[p_point->x][p_point->y] = true;
}

int copyBuffer(Buffer* target_buffer, Buffer* data_to_write_to_target)
{
  for (int x = 0; x != 8; x++)
  {
    for (int y = 0; y != 8; y++)
    {
      target_buffer->contents[x][y] = data_to_write_to_target->contents[x][y];
    }
  }
}

void flipBufferVertically(Buffer* target_buffer)
{
  Buffer temp;

  for (int x = 0; x != 8; x++)
  {
    for (int y = 0; y != 8; y++)
    {
      temp.contents[x][y] = target_buffer->contents[7 - y][x];
    }
  }

  copyBuffer(target_buffer, &temp);
}

LedControl lc = LedControl(12, 11, 10, 1);

struct Ray
{
  Point position = {3, 3};
  Direction dir = {0, 0};

  int times_stepped = 0;

  Point points_passed[6];

  void step()
  {
    times_stepped++;

    position.x += dir.x;
    position.y += dir.y;
  }
};

Ray test_ray;

int swapBuffers(Buffer* buffer_a, Buffer* buffer_b)
{
  digitalWrite(13, HIGH);

  Buffer temp;

  copyBuffer(&temp, buffer_a);
  copyBuffer(buffer_a   , buffer_b);
  copyBuffer(buffer_b   , &temp);
}

void blendBuffers(Buffer* output_buffer,
                  Buffer* buffer_a,
                  Buffer* buffer_b)
{
  for (int x = 0; x != 8; x++)
  {
    for (int y = 0; y != 8; y++)
    {
      if(buffer_a->contents[x][y] == true
        ||
        buffer_b->contents[x][y] == true)
      {
        output_buffer->contents[x][y] = true;
      }
    }
  }
}

struct BufferStack
{
  Buffer buffers[5];

  int buffer_count;

  void addNewBuffer(Buffer* n_buffer)
  {
    if (buffer_count == 4)
    {
      return;
    }
    buffer_count++;
    buffers[buffer_count] = n_buffer;
  }

  void clearBuffers()
  {
    for (int buffer_index = 0; buffer_index != buffer_count; buffer_index++)
    {
      delete buffers[buffer_index];
    }
    buffer_count = 0;
  }
};

BufferStack main_buffer_stack;

Point test_point = {4, 4};

float accelX, accelY, accelZ;

Point movement_vector = {accelX, accelY};

Buffer back_buffer;
Buffer screen_buffer;
Buffer shape_buffer_a;
Buffer shape_buffer_b;

void setup()
{
  Serial.begin(9600);

  //wake up the MAX72XX from power-saving mode
  lc.shutdown(0, false);
  //set a medium brightness for the Leds
  lc.setIntensity(0, 2);

  digitalWrite(2, HIGH);

  //drawHeart(screen_buffer);
  //drawScreenBuffer(&lc, screen_buffer);
  //clearBuffer(screen_buffer);
  //drawHeart(shape_buffer);

  accelero.begin();

  test_ball.position = {3, 3};

  drawCircle(&shape_buffer_a);
  drawCircle(&shape_buffer_b);

  shiftBufferUp(&shape_buffer_a);
  shiftBufferUp(&shape_buffer_a);
  shiftBufferUp(&shape_buffer_a);

  shiftBufferDown(&shape_buffer_b);
  shiftBufferDown(&shape_buffer_b);
  shiftBufferDown(&shape_buffer_b);
}

void loop()
{


swapBuffers(&screen_buffer, &back_buffer);
drawScreenBuffer(&lc, &screen_buffer);
clearBuffer(&screen_buffer);

//shiftBufferToLeftWithWrap(&shape_buffer_a);
//drawCircle(&shape_buffer_a);
blendBuffers(&back_buffer, &shape_buffer_a, &shape_buffer_b);

shiftBufferToLeftWithWrap(&shape_buffer_a);

shiftBufferToRightWithWrap(&shape_buffer_b);

delay(120);
}