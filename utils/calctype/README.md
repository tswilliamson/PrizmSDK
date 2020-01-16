# calctype
Typeface tools for color calculators using sub-pixel rendering concepts

# Font Generation
First generate a font using BMFont.exe. It has a ton of options for laying out glyphs. You'll want to fit your glyphs in a 256x256 image to construct a 10 kb sized font on the calculator, though you can make them as big as you want based on your needs. Save the font out to its own folder in the calctype/fonts folder OR in your project folder. 

Run the calctype tool (contained in Tool/bin/Release/CalcTyper.exe), and select Font->Process BMFont Output... and point it to your image output by BMFont. The program should load up your font image if successful, and automatically create a .c/.h for inclusion in your project. Be aware that you will have to fix the pathing in the .h file if you intend on using the font in your project folder.