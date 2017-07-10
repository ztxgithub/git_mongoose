#./git_push_orgin.sh $1 $2
#./git_push_orgin.sh DEPLOY.md "加commit注释"
echo $1
git add $1
git commit -m $2
git push origin master
