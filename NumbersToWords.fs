//Gets the "length" of a number, i.e.: 12345 will equal 5, for being
//comprised of 5 numbers.
let rec GetLength n =
    let length = 1;
    match n with
    | _ when n >= 10 -> length + GetLength (n/10)
    | _ -> length

//Takes a number and converts it into groups of three.
//Returns a list of tuples, the first part is the group number
//The second is the position of that number in the overall list
let rec GetGroups n =
    let length = GetLength n
    let e = int(float(length) / 3.0 - 0.1)
    let power = int(System.Math.Pow(1000.0, float(e)))
    let group = n / power
    match length with
    | _ when length > 3 -> (group, e) :: GetGroups (n - group * power)
    | _ -> [(group, 0)]

//Takes a number under 100 and turns it into an English word
//Returns a word if matched or a null if not
let NumberToWord n =
    match n with
        | 1 -> "One"
        | 2 -> "Two"
        | 3 -> "Three"
        | 4 -> "Four"
        | 5 -> "Five"
        | 6 -> "Six"
        | 7 -> "Seven"
        | 8 -> "Eight"
        | 9 -> "Nine"
        | 10 -> "Ten"
        | 11 -> "Eleven"
        | 12 -> "Twelve"
        | 13 -> "Thirteen"
        | 14 -> "Fourteen"
        | 15 -> "Fifteen"
        | 16 -> "Sixteen"
        | 17 -> "Seventeen"
        | 18 -> "Eighteen"
        | 19 -> "Nineteen"
        | 20 -> "Twenty"
        | _ when n > 20 && n < 30 -> "Twenty-"
        | 30 -> "Thirty"
        | _ when n > 30 && n < 40 -> "Thirty-"
        | 40 -> "Fourty"
        | _ when n > 40 && n < 50 -> "Fourty-"
        | 50 -> "Fifty"
        | _ when n > 50 && n < 60 -> "Fifty-"
        | 60 -> "Sixty"
        | _ when n > 60 && n < 70 -> "Sixty-"
        | 70 -> "Seventy"
        | _ when n > 70 && n < 80 -> "Seventy-"
        | 80 -> "Eighty"
        | _ when n > 80 && n < 90 -> "Eighty-"
        | 90 -> "Ninety"
        | _ when n > 90 && n < 100 -> "Ninety-"
        | _ -> null

//Takes a tuple from GetGroups and converts it to a list of words
//Returns a list of strings representing each word in a group
//without punctuation
let rec GetWords (n, g) =
    let i = int(System.Math.Pow(10.0, float(GetLength n - 1)))
    let words = []
    let AddSize = match g with | 3 -> " Billion" | 2 -> " Million" | 1 -> " Thousand" | _ -> ""
    let words = match i with
                | 100 -> words @ [NumberToWord(n/i); " Hundred"] @ if n % i > 0 then [" and "] @ GetWords((n%i), g) else [AddSize]
                | 10 -> words @ [NumberToWord n + (if n > 20 then NumberToWord (n%i) else ""); AddSize]
                | 1 -> words @ if g = 0 then [NumberToWord n] else [NumberToWord n; AddSize]
                | _ -> words
    words
    
//Takes a number and turns it into a proper english sentence
let Say n =
    //Gets the list of lists of strings with each word in it
    let wordGroups = match n with
                        | _ when n > 0 -> List.map GetWords (GetGroups n)
                        | _ -> [["Zero"]]
    
    //Turns that list of lists of strings into a list of strings, 
    //adding punctuation as it goes.
    let wordList = []
    let rec buildList (groups:string list list) =
        wordList @ groups.Head @ 
                if groups.Tail.Length = 0 || groups.Tail.Head.Head = null then ["."] 
                else 
                    (if groups.Tail.Length = 1 && groups.Tail.Head.Length = 1 then [" and "] else [", "]) @ (buildList groups.Tail)
    let wordList = buildList wordGroups

    //Converts the list of strings into one string.
    let sentence = ""
    let rec buildSentence (words: string list) =
        sentence + words.Head + if words.Tail.Length > 0 then (buildSentence words.Tail) else ""

    //Prints out the string with a linebreak.
    printf "%s\n" (buildSentence wordList)