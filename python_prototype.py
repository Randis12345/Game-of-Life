import pygame as p

p.init()

SS = (600,600)
screen = p.display.set_mode(SS)
clock = p.time.Clock()

mousepressed = [False]*7
mousepos = (0,0)
lastmousepos = -1
camera = [SS[0]/2,SS[1]/2]
zoom = 10
paused = True

m1 = []#[(22,20),(21,20),(20,20)]#[(0,0),(1,0),(2,0),(2,-1),(1,-2)]
processed = []
m2 = []
timesincelastupdate = 0
updatefreq = 2#1/20

def getsurround(pos):
    return (
        (pos[0]-1,pos[1]+1),
        (pos[0],pos[1]+1),
        (pos[0]+1,pos[1]+1),
        (pos[0]-1,pos[1]),
        (pos[0]+1,pos[1]),
        (pos[0]-1,pos[1]-1),
        (pos[0],pos[1]-1),
        (pos[0]+1,pos[1]-1)
        )

def cellnum(pos):
    num = 0
    for pix in getsurround(pos):
        if pix in m1: num+=1
    return num

def processcell(pos):
    if pos in processed: 
        return
    processed.append(pos)
    global m2
    alive = pos in m1
    numaround = cellnum(pos)
    if (alive and (numaround == 2 or numaround == 3)) or (not alive and numaround == 3): 
        m2.append(pos)


running =True
while running:
    dt = clock.tick(144)/1000
    #print(1/dt)
    for event in p.event.get():
        if event.type == p.QUIT:
            running = False
        elif event.type == p.KEYDOWN:
            if event.unicode == " ":
                paused = not paused
        elif event.type == p.MOUSEBUTTONDOWN:
            mousepressed[event.button-1] = True
        elif event.type == p.MOUSEBUTTONUP:
            mousepressed[event.button-1] = False
        elif event.type == p.MOUSEMOTION:
            mousepos = event.pos
        elif event.type == p.MOUSEWHEEL:
               amount = event.y*14*dt*zoom
               if 1-zoom > amount: amount = 1-zoom
               for i in range(2): camera[i] = mousepos[i] - (1+amount/zoom)*(mousepos[i]-camera[i])
               zoom += amount

        
    if mousepressed[2]:
        if lastmousepos != -1:
            camera[0]+= mousepos[0]-lastmousepos[0]
            camera[1]+= mousepos[1]-lastmousepos[1]
        lastmousepos = mousepos
    else:
        lastmousepos = -1

    timesincelastupdate+=dt
    # draw
    if paused and mousepressed[0]:
        #print((mousepos[0]-camera[0])/zoom)
        pos = []
        for i in range(2):
            pos.append(int((mousepos[i]-camera[i])/zoom))
            if pos[i] < 0:  pos[i]-=1
        pos = tuple(pos)
        if not pos in m1:
            m1.append(pos)
    # update
    if timesincelastupdate > updatefreq and not paused:
        #print(m1)
        for cell in m1:
            processcell(cell)
            for arcell in getsurround(cell): 
                processcell(arcell)
        processed = []
        m1 = m2.copy()
        m2 = []
        timesincelastupdate = 0

    screen.fill("black")
    for cell in m1:
        p.draw.rect(screen,"white",[cell[0]*zoom + camera[0],cell[1]*zoom + camera[1],zoom*1.05,zoom*1.05])

    p.display.update()
    clock.tick(60)
p.quit()