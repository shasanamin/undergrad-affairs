import Data.List
import Data.Char

data Token = Word String | Blank | HypWord String deriving (Eq, Show)

type Line = [Token]

-- To remove hyphen. Function assumes that hyphen present in string
removeHyp::String->String
removeHyp = \s -> filter (not.(`elem` "-")) s 

addHyp::String->String
addHyp = \s -> s ++ "-"

strLen::String->Int
strLen = \s ->
    case s of
        [] -> 0
        [x] -> 1
        x:xs -> 1 + strLen xs

str2line::String->Line
str2line = \s ->
    case words(s) of 
        [] -> []
        [x] | '-' `elem` x -> HypWord (removeHyp x):[]
        [x] | ' ' `elem` x -> Blank:[] 
        [x] -> Word x:[]
        x:xs | '-' `elem` x -> HypWord (removeHyp x):str2line (unwords xs)
        x:xs | ' ' `elem` x -> Blank:str2line (unwords xs)
        x:xs -> Word x:str2line (unwords xs)

line2str::Line->String
line2str = \l ->
    case l of
        [] -> []
        [HypWord x] -> addHyp x
        [Blank] -> " "
        [Word x] -> x
        HypWord x:xs -> addHyp x ++ " " ++ line2str xs
        Blank:xs -> " " ++ line2str xs  -- " " ++ " " ++ line2str xs ?
        Word x:xs -> x ++ " " ++ line2str xs

-- Length of Token
tokLen::Token->Int
tokLen = \t ->
    case t of
        Blank -> 1
        Word x -> strLen x
        HypWord x -> 1 + strLen x

-- Length of Line i.e. a list of Tokens
lineLen::Line->Int
lineLen = \l ->
    case l of
        [] -> 0
        [x] -> tokLen x 
        x:xs -> 1 + tokLen x + lineLen xs 

-- Break line into pairs so that it is not larger than given width
breakLine::Int->Line->(Line,Line)
breakLine = \width -> \lin ->
    case lin of 
        [] -> ([],[])
        _ | (lineLen lin) <= width -> (lin,[Blank])
        x:xs | tokLen x <= width -> 
            let (firstL, secondL) = breakLine (width - (tokLen x) - 1) xs  -- is -1 required?
            in ([x] ++ firstL, secondL)
        _ -> ([], lin)

-- Given all parts of a word, mergers produces a pair of all possible ways to break up the word.
mergers::[String]->[(String,String)]
mergers = \sList ->
    case sList of
        [] -> [([], [])]
        [x,x1] -> [(x,x1)]
        x:xs -> [(x,concat xs)] ++ zip (map (x ++) (map fst (mergers xs))) (map snd (mergers xs))

-- enHyp provides hyphenation map (referred to as hypMap in functions) i.e., possible breakdowns of word where hyphen can be added
enHyp = [("controls",["co","nt","ro","ls"]), ("future",["fu","tu","re"]), ("present",["pre","se","nt"])]

type HyphenationMap = [ (String, [String]) ]

-- helper function to find possible breakdown of provided word from hyphenation map e.g. ["co","nt","ro","ls"] for "controls" from enHyp
possWaysBreak::String -> HyphenationMap -> [String]
possWaysBreak = \myWord -> \hypMap -> 
    let relevantHyp = find (\(x,_) -> x == myWord) hypMap
    in case relevantHyp of
        Just a -> snd a
        Nothing -> [myWord]

str2word::String->Token
str2word = \s -> Word s 

str2hypword::String->Token 
str2hypword = \s -> HypWord s 

{-Possible breakdowns of word with hyphenation
Have to deal with case where word has trailing punctuation, thus filtering for alphabets
From the breakdown combinations obtained through possWaysBreak function, first portion needs to be hyphenated (HypWord) and 
second portion needs to be put as ordinary word (Word) plus any removed trailing punctuation has to be added-}
hyphenate::HyphenationMap -> Token -> [(Token, Token)]
hyphenate = \hypMap -> \t ->
    case t of
        -- deal with HypWord and Blank if need be
        Word myWord -> 
            let breakCombinations = mergers (possWaysBreak (filter (isAlpha) myWord) hypMap) 
            in zip (map (str2hypword.fst) breakCombinations) (map str2word  (map (++ (filter (not.isAlpha) myWord)) (map snd breakCombinations)) )

-- Adding a Line (i.e. a List) to each element of a list of Tuples, and returning list of Lines i.e. combination of one Line with every Tuple (of Tokens)
addFromListToTuple::[(Token,Token)] -> Line -> [Line]
addFromListToTuple = \toktokList -> \lin -> 
    case toktokList of 
        [] -> []
        x:xs -> [fst x:[snd x] ++ lin] ++ (addFromListToTuple xs lin)  -- converting tuple to list in start, then recursion

{-lineBreaks breaks line so that width doesn't exceed provided width.
This is similar to breakLine implemented earlier, just a bit more complex
If empty, return empty
Else if line doesn't exceed provided width, put everything in first line
Else if first token length is less than or equal to provided width, append first token to whatever is added to first line 
(found through recursively applying function on remaining line; have to subtract length of Token and a space to get updated width requirement)
Else if even first word too long, hyphenate that word and get all possible combinations. 
However, need to see which hyphenations obey width requirement so will use (/map on list) our ordinary breakLine function (some complex filtering might have done the job too)-}
lineBreaks::[ (String, [String]) ] -> Int -> Line -> [(Line, Line)]
lineBreaks = \hypMap -> \width -> \lin ->
    case lin of 
        [] -> [([], [])]
        _ | (lineLen lin) <= width -> [(lin, [])]
        x:xs | tokLen x <= width -> 
            let otherBreaks = lineBreaks hypMap (width - (tokLen x) - 1) xs
            in zip (map ([x] ++) (map fst otherBreaks)) (map snd otherBreaks)
        x:xs -> -- x is larger than desired width. Will have to hyphenate it.
            let possibleLists = (addFromListToTuple (hyphenate hypMap x) xs) 
            in map (breakLine width) possibleLists

{-insert a character at every possible place in a string
its type dictates that we won't be able to use this directly for our lines
can possibly work around this using str2line and line2str, but I'm better off making a new function later, 
only using this simpler function as template to build upon-}
insertions::Char->String->[String]
insertions = \c -> \cList ->
    case cList of
        [] -> []
        x:xs -> [c:cList] ++ map (x:) (insertions c xs)

{-Blank is a token, Line is a list of tokens (much like char and list of char in insertions)
Insert numB number of blanks in every possible way 
Code dictates that no Blank at end
Blank at start (which would be filtered later)
Each case is repeated (because maybe Blank1, Blank2 considered differently whereas we view Blank1 = Blank2 = Blank)-}
insertBlanksUnfiltered::Int->Line->[Line]
insertBlanksUnfiltered = \numB -> \lin ->
    case (numB, lin) of
        (_, []) -> []
        (1, x:xs) -> [Blank:lin] ++ map (x:) (insertBlanksUnfiltered 1 xs)
        (_, x:xs) -> concat (map (insertBlanksUnfiltered (numB-1)) (insertBlanksUnfiltered 1 lin))

-- [Line] is a list of list (have to check whether head of head of input is Blank or not); handling it accordingly
removeBlankAtStart::[Line]->[Line]
removeBlankAtStart = \linList ->
    case linList of
        [] -> [] 
        [x] | (head x) == Blank -> []
        x:xs | (head x) == Blank -> removeBlankAtStart xs
        x:xs -> x:removeBlankAtStart xs

removeRepeated::[Line]->[Line]
removeRepeated = \linList ->
    case linList of
        [] -> [] 
        [x] -> [x]
        x:xs | x `elem` xs -> removeRepeated xs
        x:xs -> x:removeRepeated xs

insertBlanks::Int->Line->[Line]
insertBlanks = \numB -> \lin -> removeRepeated (removeBlankAtStart (insertBlanksUnfiltered numB lin))

-- output: [numTokenBeforeFirstBlank, numTokenBetweenFirstAndSecondBlank, numTokenBetweenSecondAndThirdBlank, numTokenBetweenThirdandFourthBlank...]
blankDistances::Line->[Double]
blankDistances = \lin -> 
    case lin of 
        [x] -> [1]
        x:xs | x == Blank -> 0:blankDistances xs
        x:xs | x /= Blank -> (1 + head (blankDistances xs)):tail (blankDistances xs)

-- General function for length of list
listLen::[a] -> Double
listLen = \list ->
    case list of
        [] -> 0
        x:xs -> 1 + listLen xs

-- Summing a list of doubles
dListSum::[Double] -> Double
dListSum = \list ->
    case list of
        [] -> 0
        x:xs -> x + dListSum xs

-- Finding average of a list of doubles
avg::[Double]->Double
avg = \list ->
    case list of 
        [] -> error "Average can't be computed on empty list"
        _ -> (dListSum list) / (listLen list)

-- (x_i-avg)^2 for all elements of list
sumOfSquaredDiffFromAvg::[Double]->Double->Double
sumOfSquaredDiffFromAvg = \list-> \avgList ->
    case list of
        [] -> error "Empty List"
        [x] -> (x - avgList)*(x - avgList)
        x:xs -> (x - avgList)*(x - avgList) + (sumOfSquaredDiffFromAvg xs avgList)

-- Finding variance of a list of doubles
var::[Double]->Double
var = \list ->
    case list of 
        [] -> error "Variance can't be computed on empty list"
        _ -> (sumOfSquaredDiffFromAvg list (avg list)) / (listLen list)

data Costs = Costs Double Double Double Double deriving (Eq, Show) 

blankCost::Line -> Double
blankCost = \lin ->
    case lin of 
        [] -> 0
        x:xs | x == Blank -> 1 + blankCost xs
        x:xs -> blankCost xs

blankProxCost::Line -> Double 
blankProxCost = \lin -> (listLen lin) - avg (blankDistances lin)

blankUnevenCost::Line -> Double 
blankUnevenCost = \lin -> var (blankDistances lin)

hypCost::Line -> Double
hypCost = \lin ->
    case lin of
        _ | '-' `elem` (line2str ([last lin])) -> 1  -- Rather indirect way to say return 1 if Hypword
        _ -> 0

-- treating defaultCosts as weights
defaultCosts = Costs 1 1 1 1

lineBadness::Costs -> Line -> Double
lineBadness = \(Costs bC bPC bUC hC) -> \lin -> bC*(blankCost lin) + bPC*(blankProxCost lin) + bUC*(blankUnevenCost lin) + hC*(hypCost lin)

setLineWidth::Int -> Line -> [Line]
setLineWidth = \width -> \lin ->
    case width of 
        _ | width == (lineLen lin) -> [lin]
        _ -> insertBlanks (width - (lineLen lin)) lin 

-- Assumption: List of Equal Sizes 
twoListsToListOfTuple::[a] -> [b] -> [(a,b)]
twoListsToListOfTuple = \s1 -> \s2 -> 
    case (s1, s2) of 
        ([],[]) -> []
        (x:xs, y:ys) -> (x,y):twoListsToListOfTuple xs ys

-- form firstLine, respective secondLine pairs (while not removing costs)
-- till last token of each first line same, they are essentially originating from same first line; second line unchanged for these
matchSecondLine::[(Line,Double)] -> [Line] -> [((Line, Double), Line)]
matchSecondLine = \firstLBadT -> \secLList -> 
    case (firstLBadT, secLList) of 
        ([x], [y]) -> [(x,y)]
        (x:x1:xs, y:ys) | (last (fst x) == last (fst x1)) -> (x,y):(matchSecondLine (x1:xs) secLList)
        (x:x1:xs, y:ys) -> (x,y):(matchSecondLine (x1:xs) ys)
        _ -> []

-- to find line with minimum badness scores
-- fancy input type because don't want to lose second line (or get it back through indirect way like getting index of extract tuple)
minTuple::[ ((Line, Double), Line) ] -> ((Line, Double), Line)
minTuple = \tupList ->
    case tupList of
        [x] -> x 
        [x,x2] | (snd (fst x)) < (snd (fst x2)) -> x
        [x,x2] -> x2
        x:x2:xs | (snd (fst x)) <= (snd (fst x2)) -> minTuple(x:xs)
        x:x2:xs | (snd (fst x)) > (snd (fst x2)) -> minTuple(x2:xs)
        [] -> (([],0),[])

wordToString::Token -> String
wordToString = \t -> 
    case t of 
        Word x -> x 
        _ -> error "wordToString undealt case"

hypwordToString::Token -> String
hypwordToString = \t -> 
    case t of 
        HypWord x -> x 
        _ -> error "hypwordToString undealt case"

isHypWord::Token -> Bool
isHypWord = \t ->
    case t of 
        HypWord x -> True
        _ -> False

lineCorrection::Line -> Line 
lineCorrection = \lin ->
    case lin of 
        x:x1:xs | isHypWord x -> str2line ((hypwordToString x) ++ (wordToString x1) ++ " " ++ (line2str xs))
        _ -> lin 


bestLineBreak::Costs -> HyphenationMap -> Int -> Line -> Maybe (Line, Line)
bestLineBreak = \c -> \hypMap -> \width -> \lin ->
    let possBreakdowns = lineBreaks enHyp width lin
        possFirstLine = map fst possBreakdowns
        possSecondLine = map snd possBreakdowns
        possSameLengthFL = concat $ map (setLineWidth width) possFirstLine  -- add Blanks so that each line of same length
        firstLineBadness = map (lineBadness c) possSameLengthFL
        firstLineBadnessTuple = twoListsToListOfTuple possSameLengthFL firstLineBadness
        -- possSecondLine of lesser length than firstLineBadnessTuple. matchSecondLine attaches relevant second line to each first line with added blanks for fixed length (constant width)
        badnessTuple = matchSecondLine firstLineBadnessTuple possSecondLine
        bestTuple = minTuple badnessTuple
    in case lin of
        _ | sum (map lineLen possFirstLine) == 0 -> Nothing  -- condition checks that all possible first lines are empty
        _ -> Just (fst (fst bestTuple), snd bestTuple)

justifyLine::Costs -> HyphenationMap -> Int -> Line -> [Line]
justifyLine = \c -> \hypMap -> \width -> \lin -> 
    case (bestLineBreak c hypMap width lin) of 
        -- Just (firstL, secondL) -> [secondL]
        Just (firstL, []) -> [firstL]
        Just (firstL, secondL) | (lineLen secondL) > width -> firstL:justifyLine c hypMap width (lineCorrection secondL)
        Just (firstL, secondL) -> firstL:[lineCorrection secondL]
        Nothing -> []

justifyText::Costs -> HyphenationMap -> Int -> String -> [String]
justifyText = \c -> \hypMap -> \width -> \txt -> map line2str (justifyLine c hypMap width (str2line txt))

-- testing
text = "He who controls the past controls the future. He who controls the present controls the past."
main = putStr $ unlines $ justifyText defaultCosts enHyp 15 text