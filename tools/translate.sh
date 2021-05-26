word=`xclip -out`
#echo "$word" >> his.txt
echo -e `sdcv -n -c "$word"`
read -p "同步到手机?" oulu
if [ "$oulu" = "y" ]; then
curl -H 'Content-Type: application/json' -H 'Authorization: 认证码' -d '{"id":"0","language":"en","words":["'$word'"]}' https://api.frdic.com/api/open/v1/studylist/words
fi
echo
