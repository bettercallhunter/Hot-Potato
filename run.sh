#!/bin/bash
osascript -e 'tell app "Terminal"
    do script "~/Repository/650-project3/ringmaster 5000 2 10"
end tell'
sleep 1
for (( c=0; c<2; c++ ))
do
sleep 1
osascript -e 'tell app "Terminal"
    do script "~/Repository/650-project3/player 127.0.0.1 5000"
end tell'
done