#include "types.h"
#include "stat.h"
#include "user.h"

#define NREF 20
#define MAXFRAMES 5

enum { FIFO, LRU, OPTIMAL };
static int refs[NREF] = {7,0,1,2,0,3,0,4,2,3,0,3,2,1,2,0,1,7,0,1};
static char *names[] = {"FIFO", "LRU", "Optimal"};

static void
showframes(int *frames, int n)
{
  int i;
  printf(1, "[");
  for(i = 0; i < n; i++){
    if(i) printf(1, " ");
    if(frames[i] < 0) printf(1, "-");
    else printf(1, "%d", frames[i]);
  }
  printf(1, "]");
}

static int
simulate(int algorithm, int nframes, int trace)
{
  int frames[MAXFRAMES], age[MAXFRAMES];
  int i, j, faults = 0, tick = 0;
  for(i = 0; i < nframes; i++){
    frames[i] = -1;
    age[i] = -1;
  }
  if(trace){
    printf(1, "\n%s with %d frames\n", names[algorithm], nframes);
    printf(1, "Step Ref Frames       Result\n");
  }
  for(i = 0; i < NREF; i++){
    int page = refs[i], slot = -1, hit = 0;
    tick++;
    for(j = 0; j < nframes; j++)
      if(frames[j] == page){ slot = j; hit = 1; break; }
    if(hit){
      if(algorithm == LRU) age[slot] = tick;
    } else {
      int victim = -1;
      faults++;
      for(j = 0; j < nframes; j++)
        if(frames[j] < 0){ victim = j; break; }
      if(victim < 0){
        if(algorithm == FIFO || algorithm == LRU){
          victim = 0;
          for(j = 1; j < nframes; j++)
            if(age[j] < age[victim]) victim = j;
        } else {
          int farthest = -1;
          for(j = 0; j < nframes; j++){
            int next;
            for(next = i + 1; next < NREF; next++)
              if(refs[next] == frames[j]) break;
            if(next == NREF){ victim = j; break; }
            if(next > farthest){ farthest = next; victim = j; }
          }
        }
      }
      frames[victim] = page;
      age[victim] = tick;
    }
    if(trace){
      printf(1, "%d  %d  ", i + 1, page);
      showframes(frames, nframes);
      printf(1, "  %s\n", hit ? "Hit" : "Fault");
    }
  }
  return faults;
}

int
main(void)
{
  int a, f;
  printf(1, "Reference string: 7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1\n");
  for(a = FIFO; a <= OPTIMAL; a++)
    printf(1, "%s total faults with 3 frames: %d\n", names[a], simulate(a, 3, 1));
  printf(1, "\nFrames Algorithm Faults Hits Hit ratio Fault ratio\n");
  for(f = 3; f <= 5; f++){
    for(a = FIFO; a <= OPTIMAL; a++){
      int faults = simulate(a, f, 0);
      int hits = NREF - faults;
      printf(1, "%d  %s  %d  %d  %d.%d%%  %d.%d%%\n",
             f, names[a], faults, hits, hits * 100 / NREF,
             (hits * 1000 / NREF) % 10, faults * 100 / NREF,
             (faults * 1000 / NREF) % 10);
    }
  }
  printf(1, "Optimal has the fewest faults for this reference string.\n");
  exit();
}
