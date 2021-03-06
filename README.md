# R-Link
This repository contains the sourcecode for **aimms4r c++ R package** & **r4aimms c++ project**. These two projects constitute the **R-link project**, a way to allow AIMMS to *execute user specified R code* and *pass data between AIMMS and the R runtime environment*, **extending** AIMMS optimizations & sparse execution capabilities with R's rich library ecosystem.    

# Table Of Contents
* [The R programming language](#the-r-programming-language)
* [R4AIMMS](#r4aimms)
* [Requirements](#requirements)
* [Build R-Link](#build-r-link)
    * [Windows](#windows)
    * [Linux](#linux)  
* [Install R-Link](#install-r-link)
    * [Windows](#windows-1)
    * [Linux](#linux-1)  
* [R::executeScript Procedure](#rexecutescript-procedure)
* [Data passing between AIMMS and the R runtime environment](#data-passing-between-aimms-and-the-r-runtime-environment)
    * [Aimms4r::GetData](#aimms4rgetdata)
        * [Data Frame Form](#data-frame-form)
        * [Aimms Storage Types and R Data Frame Column Data Types](#aimms-storage-types-and-r-data-frame-column-data-types)
    * [Aimms4r::SetData](#aimms4rsetdata)
        * [Storage Type Conversions when assigning R data to AIMMS (functionality partially there)](#storage-type-conversions-when-assigning-r-data-to-aimms-functionality-partially-there)
* [R Object Lifetime](#r-object-lifetime)
* [AIMMS to R data passing and vice versa example](#aimms-to-r-data-passing-and-vice-versa-example)
* [AIMMS4R package documentation](#aimms4r-package-documentation)
    * [GetData](#getdata)
    * [SetData](#setdata)
    * [Examples](#examples-1)
* [License](#license)
* [NOTICE](#notice)

# The R programming language
*[R](https://www.r-project.org/)* is an open source programming language and software environment for statistical computing that is broadly used among statisticians, econometrists and data miners. R implements a wide variety of statistical techniques such as linear and nonlinear modeling, time-series analysis, classification and clustering. 

# R4AIMMS
*R4AIMMS* is an AIMMS system library that allows AIMMS to execute R user-defined code and provides data-passing facilities from AIMMS to the R runtime and vice versa. R4AIMMS extends the AIMMS experience with R’s universe of statistical analysis tools, ultimately allowing AIMMS users to write and use their own R scripts/libraries through AIMMS.

# Requirements
The R4AIMMS system library **requires** that the *R programming language* is already installed on the target machine along with the **Rcpp R package**. The Rcpp package requires R’s version to be **>= 3.0.0 and <= 3.3.0 ** and can be installed from within R by typing ```install.packages(“Rcpp”)```.

# Build R-Link
## Windows
You can build the R-Link by executing the BATCH file build.bat with the following arguments:
* __architecture :__ *Win32* or *x64*
* __configuration :__ *Release* or *Debug*
* __compiler :__ *vc120*  

## Build example:
**build.bat Win32 Release vc120**

## Linux
You can build the R-Link by executing the sh script build.sh with the following argument:
* __configuration :__  *Debug64* or *Release64*  

### Build example:
**sh build.sh Release64**

# Install R-Link
## Windows
After building the R-Link (or acquiring the binaries), you will need to place the **R4AIMMS** folder
in your AIMMS installation folder, under the **Libraries** folder. If you built R-Link yourself, the 
folder R4AIMMS will be located under the **target/{{compiler}}_{{architecture}}_{{configuration}}/Libraries**

### Example location:
```
$dir C:\Users\RAIMMSDeveloper\AppData\Local\AIMMS\IFA\Aimms\4.34.1.910-x86
Api  Bin  Doc  Help Lib  **Libraries**  Modules  Solvers Templates Tutorial WebUIDev WebUIService

$dir C:\Users\RAIMMSDeveloper\AppData\Local\AIMMS\IFA\Aimms\4.34.1.910-x86\Libraries
AIMMSForecasting  AimmsPro AimmsProUI AimmsWebUI AIMMSXLLibrary ==> R4AIMMS <==
```

## Linux
After building the R-Link (or acquiring the binaries), you will need to place the **R4AIMMS** folder
in your AIMMS installation folder, under the **Libraries** folder. If you built R-Link yourself, the 
folder R4AIMMS will be located under the **target/{{compiler}}_{{architecture}}_{{configuration}}/Libraries**

### (RPM installed AIMMS) Example location:
```
$ls /usr/local/Aimms/4.23.10000
Api  Bin  Doc  Lib  **Libraries**  Modules  Solvers

$ls /usr/local/Aimms/4.23.10000/Libraries/
AIMMSForecasting  AimmsPro AimmsProUI AimmsWebUI AIMMSXLLibrary ==> R4AIMMS <==
```

### (IFA installed AIMMS) Example location:
```
$ls /home/RAIMMSDeveloper/.Aimms/4.34.4.934
Api  Bin  Doc  Lib  **Libraries**  Modules  Solvers

$ls /home/RAIMMSDeveloper/.Aimms/4.34.4.934/Libraries/
AIMMSForecasting  AimmsPro AimmsProUI AimmsWebUI AIMMSXLLibrary ==> R4AIMMS <==
```

# R::executeScript Procedure
The library exposes the **executeScript** procedure through the **R prefix**. The procedure takes a scalar string as argument, representing the R script to be executed through AIMMS. The R script can contain any arbitrary R code, such as function calls, R object declarations, library installation/import statements and so on. In case of an R error, AIMMS will notify the user by raising this error. __During R code execution through *R::executeScript* there is no visible output from the R output, thus the result to a call to R’s print function won’t be visible to the AIMMS user__.



# Data passing between AIMMS and the R runtime environment
We provide **aimms4r**, *an R library that facilitates data passing between AIMMS and the R runtime*. Aimms4r contains two R functions, the **aimms4r::GetData** and the **aimms4r::SetData**. 

# Aimms4r::GetData
**Aimms4r::GetData(scalar string: fully qualified identifier name)**

**Aimms4r::GetData** accepts as argument a *scalar string* representing the fully qualified name of an AIMMS identifier and returns either an *R data frame* in case of a multidimensional identifier, or a *scalar R object* containing the value of the AIMMS scalar. A value can have **integer**, **string**, **set element** or **double storage data type**. By calling *GetData* with the appropriate identifier’s name, the user can expose the identifier’s data content to the R universe and therefore apply any operation freely. In case there is no identifier with the name specified by the function’s input argument, the *aimms4r::GetData* function call will fail resulting in an error being raised in AIMMS. 

# Data Frame Form
Consider the *4-dimensional Identifier MultiDim(i,j,k,l)*, with i an index to the set I, j an index to the set J, k an index to the set K and l and index to set L subsequently.

    Set setI {
        Index: i;
        InitialData: {
            {1..2}
        }
    }
    Set setJ {
        Index: j;
        InitialData: {
            {1..5}
        }
    }
    Set setK {
        Index: k;
        InitialData: {
            {1..10}
        }
    }
    Set setL {
        Index: l;
        InitialData: {
            {1..20}
        }
    }
    Parameter MultiDim {
        IndexDomain: (i,j,k,l);
    }
    
After calling *__aimms4r::GetData(“MultiDim”)__*, AIMMS will return an *R data frame* holding the data of the identifier MultiDim. The data frame will contain *5 columns* where the *first 4 columns will have the name of the set of the indices (**columns setI**,**setJ**,**setK**,**setL**)*. The *last column* of the data frame will have *the same name as the AIMMS Identifier itself (**column MultiDim**)*. In the R context (during the execution of the R string code passed to *R::executeScript*), the data frame will be available and its column-specific data will be available by using the standard R data frame element access expressions. 


Consider the following R code:
```
multiDim<-aimms4r::GetData(“MultiDim”);

/*Access the first column (setI) by index value*/
iColumn<-multiDim[1]

/*Access the second column (setJ) by column name*/
jColumn<-multiDim[‘setJ’]

/*Access the data column through the dollar sign notation*/
valueColumn<-multiDim$multiDim
```

# Aimms Storage Types and R Data Frame Column Data Types
*Aimms storage types* such as **integer**, **double**, **set element** and **string** are maintained after retrieving AIMMS data through a call to *GetData*. Sets that are *subsets of the Integers set* will be retrieved as *__integer Columns__*. Normal sets will be retrieved as *__string Columns__*. Similarly, *the values of element parameters* will be retrieved **depending on their range storage type**.

# Aimms4r::SetData
**Aimms4r::SetData(r object, scalar string: fully qualified identifier name)**

**Aimms4r::SetData** accepts two arguments, the first one is an *R data frame* or an *R scalar value* and the second one is a *scalar string* that represents the fully qualified name of an existing AIMMS identifier to which we want to assign the data of the first argument. In case of a mismatch between the argument data frame’s columns and the assigned identifier’s dimensions, an error will be raised and reported by AIMMS. Additionally, **the function does not add new elements to the sets, in case it encounters a column element that does not exist in the corresponding set**. In that case, an error will be raised by AIMMS, informing the user about that unknown element.

# Storage Type Conversions when assigning R data to AIMMS (functionality partially there)
The following conversion table holds when data is assigned to an identifier by using the aimms4r::SetData function. When we want to assign element data to an element identifier from R, the element value should be within range, for the assignment to be successful. Otherwise, it will fail and AIMMS will issue an error.

FROM/TO | Element | Integer | Double | String
------- | ------- | ------- | ------ | -------
Integer | OK if range=integer | OK | OK | FAIL
Double | OK if range=string | FAIL | OK | FAIL
String | OK if range=string | FAIL | FAIL | OK

 
# R Object Lifetime
Consider the following AIMMS statements where the R::executeScript procedure is used:
1.  ```R::executeScript("data<-aimms4r::GetData('Population')");```
2.	```R::executeScript("population<-data[3]");```
3.	```R::executeScript("population <- population * 100;");```
4.	```R::executeScript("data[3]<-population");```
5.	```R::executeScript("aimms4r::SetData(data, 'Population')");```

In line 1, we retrieve the data of the __AIMMS identifier “Population”__ and in lines 2-4, we perform some calculations on the third column of the data frame containing the data of the identifier. In line 5, we finally assign back the processed data to the identifier. Consider that the __R data frame “data”__ (created in line 1) and the __R list “population”__ are maintained through the sequence of different calls to the R::executeScript. After the last statement, both R objects will still be visible in the R context, thus they will __still allocate some memory__. In case that the “Populations” identifier contains a big amount of data, both the “data” data frame and the “populations” list will still require big amounts of memory. __The AIMMS developer should keep that in mind and administer the memory usage consumed by the R context__. Thus, R object clean statements could be added to ensure that we are not using more memory than needed. In our case, we could issue the following statement (assuming that we do not need both R variables after assigning the data to the identifier in line 5):

6.	```R::executeScript("data<-NULL;population<-NULL;");```


# AIMMS to R data passing and vice versa example
In this example, we will show *how to use the R4AIMMS system library*, *how to execute R code through AIMMS* and *how to pass data along AIMMS to R and vice versa*.

1.	*Open AIMMS* and create a __new project named “AIMMS-R-App”__.
2.	Add a __system library “R4AIMMS”__ to the model.
3.	Create a Set “Items” with index "i" and fill it by e.g.data{item-01..item-10}.
4.	Create a Parameter InputValue(i).
5.	Create a Parameter ResultValue(i) to store the result values.
6.  Create a Procedure DetermineResultValueUsingR.
```
InputValue(i) := ord(i);
R::executeScript("data<-aimms4r::GetData('InputValue');"+
                 "data[2]<-cos(data[2]);" +
                 "aimms4r::SetData(data, 'ResultValue');");
```    
The first statement initializes the content of the *"InputValue"* identifier with the ordinal of each element of the *set “Items”*.
The second statement executes the R code which:
1.	Retrieves __a data frame named ‘data’__ containing *InputValue’s  data by calling aimms4r::GetData*.
2.	It accesses the *second column of the Data Frame* (the column that holds the actual data), applies the cosine function to each element of the column and assigns back the new data frame back to the data object.
3.	It writes the data back to the __AIMMS "ResultValue" identifier__ by calling *aimms4r::SetData*.


# AIMMS4R package documentation
# GetData

# Usage

GetData(identifierName)

# Arguments

## identifierName

An R Character variable with the name of the AIMMS identifier from which we want to retrieve data

# Value

If the identifier is a multidimensional identifier, the function will return a data frame containing the identifier’s data. Depending on the identifier’s dimensions, the data frame will first contain columns representing the set elements over which the identifier has a non-zero value (row-based tuples) and an additional column representing the actual data. The columns representing the set elements can either have a string or integer data type depending on how their sets are declared in AIMMS. The value column can have either integer, double or string data type. In case the identifier is scalar, the function will return a scalar R object with the same data type as the scalar identifier’s storage data type. Thus, the returned R object could be of integer, double, element or string data type. 

# Details

In case an unknown identifier’s name has been provided as argument, the function will raise an error.

# See Also
**aimms4r::SetData**

# Examples
```
##Assuming there is an AIMMS identifier with name “Population” in the model
data<-aimms4r::GetData(“Population”)
```




# SetData

# Usage

SetData(data, identifierName, modeForUnknownElements=0)

# Arguments

## data 

An R object containing data (i.e. 'inputdata' in below example) that, after the successful call of the function, will be *assigned to the underlying AIMMS identifier*. In case the identifier to which we want to assign the data has multiple dimensions, __the R object needs to be a data frame with columns equal to the amount of assigned identifier’s dimensions plus one,  for the value column__. The column elements should be contained in the corresponding AIMMS sets (to which the identifier indices are bound). The value column elements should have *the same storage type as the identifier’s storage type*. In case the AIMMS identifier is scalar, the R object also needs to be scalar and should have the same storage data type as the identifier.  


## identifierName 

The AIMMS identifier in which we want to store the underlying data. (i.e. 'Population' in below example)


## modeForUnknownElements

The mode can be set to __0 or 1__, where __0 is the default value__. When the mode is 0, AIMMS will raise an error in case the R data frame to be assigned to an AIMMS identifier contains tuple elements that are not present in the identifier's domain sets. When the mode is set to __1__, __new tuple elements will be added to the referenced sets & recursively to each set's rootsets__.

# Details

The function will raise an error when:
* an unknown identifier name has been provided as argument
* the R data frame does not have the same number of columns plus one as the AIMMS identifier
* the R object represents an AIMMS set element and that element is not contained in the range of the identifier (and the modeForUnknownElements argument is set to 0)

# Examples
```
##1. Showcase of SetData where modeForUnknownElements = 0
##Consider the following AIMMS declarations:
Set Cities {
    Index: c;
    InitialData: data{'Amsterdam','Haarlem','Utrecht'};
}
Parameter Population {
    IndexDomain: (c);
    InitialData: 0;
}
ElementParameter CurrentCity {
    Range: Cities;
    InitialData: ‘Utrecht’;
}

###Consider the following R code:
inputdata<-data.frame(c(‘Amsterdam’,’Haarlem’),c(779,150 ))

##Set multidimensional identifier data
SetData(inputdata,’Population’);

##Set scalar set element
SetData(‘Amsterdam’,’CurrentCity’);
```

```
##2. Showcase of SetData where modeForUnknownElements is 0 and 1
##Consider the following AIMMS declarations:

Set DutchCities {
    Index: c;
    InitialData: data{'Amsterdam','Haarlem','Utrecht','Alkmaar'};
}

Parameter Population {
    IndexDomain: (c);
    InitialData: 0;
}

###Consider the following R code:
inputdata<-data.frame(c(‘Amsterdam’,’Haarlem’,'Leeuwarden','Groningen'),c(779, 150, 108, 584 ))

##Set multidimensional identifier data
SetData(inputdata,’Population’);

##AIMMS error
==> R_tryEval: Error in SetData(inputdata, "Population"):Couldn't find set element with label Leeuwarden in Set Cities 
##Leeuwarden is not contained in the set DutchCities! 
##Let's try the above statement again but by using modeForUnknownElements equal to 1!


SetData(inputdata,’Population’,modeForUnknownElements = 1);

```
The last statement where __modeForUnknownElements__ has value equal to 1 will succeed; the AIMMS identifier Population will have values for the tuples 'Amsterdam','Haarlem',__'Leeuwarden'__,'Groningen'
and the string __'Leeuwarden' will be an element of the set DutchCities__. 


# License
[GPLv2](https://www.gnu.org/licenses/gpl-2.0.html)

# NOTICE:
Because R is **licensed GPL** (see http://www.gnu.org/licenses), we are required to release the R-Link as well under the GPL license. Our interpretation of the GPL is that we cannot distribute the R-Link together with AIMMS without causing AIMMS to be available as open source (like the R-Link is). Similarly **you cannot distribute the R-Link as a library that is part of your AIMMS project or packed together into an .aimmspack file without making your AIMMS project open source**. In order to distribute your project/.aimmspack you should refer to the R-Link as a system library and separately install the R-Link into the AIMMS system library folder (see documentation) such that distribution of the R-Link is always separate from your project.