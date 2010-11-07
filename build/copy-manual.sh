#!/bin/sh

MANUAL_PAGES="Welcome-to-Logos.md Installation.md Release-Notes.md \
	Getting-Started.md Logos-Task-Pages.md Player-Selection-Page.md \
	Tournament-Type-Page.md One-Shot-Tournament-Page.md 
	One-Shot-Tournament-Finished-Page.md Evolutionary-Tournament-Page.md \
	Evolutionary-Tournament-Finished-Page.md Sample-Players.md Glossary.md"
DATE=`date +%Y-%m-%d`

rm -f $1/_posts/*.md

for i in $MANUAL_PAGES
do
  echo "---" > "$1/_posts/$DATE-$i"
  echo "layout: post" >> "$1/_posts/$DATE-$i"
  echo "title: `echo $i | sed "s/-/ /g" | sed "s/.md//"`" >> "$1/_posts/$DATE-$i"
  echo "---" >> "$1/_posts/$DATE-$i"
  echo "" >> "$1/_posts/$DATE-$i"
  cat "doc/manual/gh-wiki/$i" >> "$1/_posts/$DATE-$i"

  # Add width to images
  sed -i "" -e "s@\(!\[.*\][(].*[)]\)@\1{:width=\"575\"}@g" "$1/_posts/$DATE-$i"
done

rm -rf $1/docs/images
cp -R doc/manual/gh-wiki/images $1/docs

