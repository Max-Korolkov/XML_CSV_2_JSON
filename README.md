# XML_CSV_2_JSON
This program is a simple XML/CSV to JSON converter.

## What it does?
Program recieves 2 input files: `sructure.xml` and `values.csv`. Assuming both files are valid, program produces an output file `values.json`, which contains both the information about the structure from XML file and values from CSV.

## Example input and output files

`structure.xml`:
```XML
<Category everyone="">
  <Name>Vehicle</Name>
  <Children>
    <Category everyone="">
      <Name>Bike</Name>
    </Category>
    <Category>
      <Name>Car</Name>
      <Children>
       <Category everyone="">
         <Name>Sedan</Name>
       </Category>
       <Category>
         <Name>Coupe</Name>
       </Category>
      </Children>
    </Category>
  </Children>
</Category>

```

`values.csv`:
```CSV
Vehicle.Car.Sedan;"Vaz 2115"
Vehicle.Car.Coupe;"Subaru BRZ"
Vehicle.Bike;"Cube Aim Pro 29"
Vehicle.Car.Coupe;"Mazda MX5"

```

These files will produce the following `values.json`:
```json
[
	{
		"category":"Bike",
		"forAll":true,
		"parentCategory":"Vehicle",
		"values":[
			"Cube Aim Pro 29"
		]
	},
	{
		"category":"Sedan",
		"forAll":true,
		"parentCategory":"Vehicle.Car",
		"values":[
			"Vaz 2115"
		]
	},
	{
		"category":"Coupe",
		"forAll":false,
		"parentCategory":"Vehicle.Car",
		"values":[
			"Subaru BRZ",
			"Mazda MX5"
		]
	}
]

```

## How it works?

Contents of the two input files are read into strings, which now contain all the data. These strings, `xml` and `csv`, are passed to `Converter`, which creates a tree using information about structure from XML file and data from CSV file.

### How XML data is parsed

XML data string is parsed into a vector of strings, which contains all tags.

Program expects for XML file to open with opening tag for base category. If tag `<Category everyone="">` is encountered, then this category `"forAll"` flag will be `true`. If `<Category>` - then flag will be set to `false`. Any other tag will result in parser creating an empty parse tree, and resulting JSON file will be empty.

All other tags will be inspected in a similiar manner. If unexpected tag is found (or required tag is missing) parse tree will be empty, and, as a result, JSON file will also be empty. After tag had been inspected and processed it is erased from vector.

After inspecting an opening tag, parser checks if there is a correct closing `</Category>` tag at the end of the vector.

Next 3 tags are expected to be `<Name>`, Name of category and `</Name>`.

Now new category is finished. It has a `forAll` boolean set, and it's Name was found. Now all that remains is to detect whether or not this category has any children.

Now, first an last tags in vector are expected to be `<Children>` and `</Children>`, respectively. Between these tags will be children categories, which are recursively parsed.

Now, structure information was successfully obtained from XML file and parse tree was created.

### How CSV data is parsed

CSV data string is expected to be structured like this: `[Path];"Value"`. 
`[Path]` consists of category names, separated by `'.'`. 
Parser looks for correct node, and then pushes data into it. If node wasn't found, then this data string is ignored.

### How data is printed as a JSON

Data for JSON is directly taken from parse tree. The output will be an array, but only nodes which have data will be printed.
