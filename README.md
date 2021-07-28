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

Contents of the two input files are read into strings, which now contain all the data. These strings, `xml` and `csv`, are passed by reference to `Converter` class, which creates a tree using information about structure from XML file and data from CSV file.

### How XML data is parsed

Program expects for XML file to open with
