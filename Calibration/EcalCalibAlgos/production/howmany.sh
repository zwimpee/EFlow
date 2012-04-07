grep processed *.stdout | awk 'BEGIN{n=0; i=1}{print i " " $3;n=n+$3; i=i+1} END{print n}'
