/*
    Created by user on 19/02/2022.
*/

#include "decodingOperations.h"

const char *keyWords[] = {"data", "string", "struct","entry","extern","mov","cmp","add","sub","lea","clr","not","inc","dec","jmp","bne","jsr","red","prn","rts","stop","r0","r1","r2","r3","r4","r5","r5","r6","r7","r8","r9","r10","r11","r12","r13", "r14", "r15"};

const char *registers[] = {"r0","r1","r2","r3","r4","r5","r6","r7","r8","r9","r10","r11","r12","r13", "r14", "r15"};

const Command commands[COMMANDS_TABLE_LENGTH] = {{"mov",0 , 0},{"cmp", 1, 0},{"add", 2, 10},{"sub", 2, 11},{"lea", 4, 0},{"clr", 5, 10},{"not", 5, 11},{"inc", 5, 12},{"dec", 5, 13},{"jmp", 9, 10},{"bne", 9, 11},{"jsr", 9, 12},{"red", 12, 0},{"prn", 13, 0},{"rts", 14, 0},{"stop", 15, 0}};


Operation* StartDecoding(char* line, int lineNum)
{
    Operation *poperation;
    poperation = (Operation *) malloc(sizeof(Operation) );
    if (!poperation)
    {
        printf("ERROR: On allocating memory.\n");
        exit(-1);
    }
    initializeOperationValues(poperation);
    decoding(poperation,line,lineNum);

    return poperation;
}

void initializeOperationValues(Operation* operation)
{
    operation->isDecoded = FALSE; /* In case of Error */
    operation->isComment = FALSE;
    operation->isLabel = FALSE;
    operation->blankLine = FALSE;
    operation->label = NULL; /* if the operation is entry so the label is the entry label, and not the label line */
    operation->isDirective = FALSE;
    operation->directiveDestiny = NOT_A_VALUE; /* String, data , external or internal*/
    operation->isRelocatableDataLabel = NOT_A_VALUE;
    operation->lines = NOT_A_VALUE; /* How many lines are needed to decoding */
    operation->dataDecoded = NULL;/*For data or string*/
    operation->SourceLineNum = NOT_A_VALUE;
    operation->sourceLabel = NULL; /*The source label */
    operation->destinationLineNum = NOT_A_VALUE;
    operation->destinationLabel = NULL; /*The destination label */
    operation->isDeclaredNonEntry = FALSE;
    operation->instructionCounterAddress = NOT_A_VALUE;
    operation->dataCounterAddress = NOT_A_VALUE;
    operation->address = NOT_A_VALUE;
    operation->originCodeLineNum = NOT_A_VALUE;
    operation->baseAddress = NOT_A_VALUE;
    operation->addressOffset = NOT_A_VALUE;
    operation->sourceBaseAddressToInsertForExtern = NOT_A_VALUE;
    operation->sourceAddressOffsetToInsertForExtern = NOT_A_VALUE;
    operation->destinationBaseAddressToInsertForExtern = NOT_A_VALUE;
    operation->destinationAddressOffsetToInsertForExtern = NOT_A_VALUE;
}

void decoding(Operation *poperation,char* line, int lineNum)
{
    int directive;
    poperation->lines=0;
    poperation->blankLine=FALSE;
    poperation->isDecoded = FALSE;
    deleteWhiteSpace(line);
    if(isBlankLine(line)==TRUE)
    {
        poperation->blankLine=TRUE;
        return;
    }
    if(isComment(line) == TRUE)
    {
        poperation->isComment = TRUE;
        poperation->isDecoded = TRUE;
        return;
    }
    if(isLabel(line)==TRUE)
    {
        extractLabel(poperation, line,  lineNum);
        poperation->isLabel=TRUE;
    }
    if(isDirectives(line) == TRUE)
    {
        poperation->isDirective = TRUE;
        directive = directiveType(line);
        switch (directive)
        {
            case ERROR:
                break;
            case DATA:
                dataDecoding(poperation,line,lineNum);
                break;
            case STRING:
                srtringDecoding(poperation,line,lineNum);
                break;
            case ENTRY:
                entryDecoding(poperation,line,lineNum);
                break;
            case EXTERN:
                externDecoding(poperation,line,lineNum);
                break;
            default:
                break;
        }
    }
    else
        extractCommand(poperation,line,lineNum);
}

void extractWord(char *line,char *word)
{
    int i=0;
    char wordTmp[N]={0};
    deleteWhiteSpace(line);
    while (line[i]&&!isspace(line[i])&&line[i]!='\n')
    {
        wordTmp[i]=line[i];
        i++;
    }
    memmove(line, line+ strlen(wordTmp), strlen(line));
    deleteWhiteSpace(line);
    strcpy(word, wordTmp);
}

int extractOperand(char *line,char *oprand,int lineNum)
{
    int i=0;
    char wordTmp[N]={0};
    deleteWhiteSpace(line);
    while (line[i]&&!isspace(line[i]))
    {
        if(line[i]==',')
            break;
        wordTmp[i]=line[i];
        i++;
    }
    memmove(line, line+ strlen(wordTmp), strlen(line));
    deleteWhiteSpace(line);
    if(line[0]!=','&&wordTmp[0]!='\0')
    {
        printError(lineNum,"Missing comma after the first operand");
        return FALSE;
    }
    memmove(line, line+1, strlen(line)); /*dele the comma*/
    deleteWhiteSpace(line);
    strcpy(oprand, wordTmp);
    return TRUE;
}

void extractCommand(Operation *poperation,char *line, int lineNum)
{
    int i,group;
    char cmd[N]={'\0'},*pcmd=cmd,firstOperand[N]={0},*pfirstOperand=firstOperand,secondOperand[N]= {0},*psecondOperand=secondOperand;
    unsigned long opcode=(1<<18);
    Command command;
    extractWord(line,pcmd);
    deleteWhiteSpace(line);
    if(!line)
    {
        poperation->blankLine=TRUE;
    }
    else
    {
        command.name[0]='\0';
        for(i = 0; i < COMMANDS_TABLE_LENGTH; i++)
        {
            if (!strcmp(pcmd, commands[i].name) )
            {
                strcpy(command.name,commands[i].name);
                command.func = commands[i].func;
                command.opcode = commands[i].opcode;
                break;
            }
        }
        if(!command.name[0])
        {
            printError(lineNum,"Unidentified command");
            return;
        }

        /* insert thr opcode to the operation struct */
        opcode+=(1<<(command.opcode));
        poperation->lineDecoded[0]=opcode;
        poperation->lines=1;/* for the opcode */


        /* determine a group to assign operation to */
        group = (i <= GROUP_1_END_INDEX)? 1 : (i <= GROUP_2_END_INDEX)? 2 : 3;
        switch(group)
        {
            case 1:
                poperation->lines++; /*for the funct and registers*/
                if(extractOperand(line,pfirstOperand,lineNum) == FALSE)
                    return;
                extractWord(line,secondOperand);
                if(strchr(secondOperand,','))
                {
                    printError(lineNum,"Unexpected character after second operand");
                    return;
                }
                if(addOperands(poperation,command,pfirstOperand,psecondOperand,lineNum)==ERROR)
                    return;
                break;
            case 2:
                poperation->lines++; /*for the funct and registers*/
                extractWord(line,secondOperand);
                if(strchr(secondOperand,','))
                {
                    printError(lineNum,"Unexpected character after operand");
                    return;
                }
                if(addOperands(poperation,command,NULL,psecondOperand,lineNum)==ERROR)
                    return;
                break;
            case 3: default:
                break;
        }
        if (lineIsEmpty(line)==FALSE)
        {
            printError(lineNum,"Contains unnecessary characters at the end of a line");
            return;
        }
    }
}

int isFirstOperandLegal(int addressingMethod, int opcode,int lineNum,char *operand)
{
    if(opcode==4&&(addressingMethod==IMMEDIATE||addressingMethod==REGISTER))
    {
        printError(lineNum,"The addressing method of the source operand can be direct or index only");
        return FALSE;
    }
    if(operand[0]=='\0')
    {
        printError(lineNum,"Missing source operand");
        return FALSE;
    }
    return TRUE;
}

int isSecondOperandLegal(int addressingMethod, int opcode, int lineNum,char *operand)
{
    if(operand[0]=='\0')
    {
        printError(lineNum,"Missing destination operand");
        return FALSE;
    }
    if(opcode==9&&(addressingMethod==IMMEDIATE||addressingMethod==REGISTER))
    {
        printError(lineNum, "The addressing method of the destination operand can be direct or index only");
        return FALSE;
    }
    else if(opcode==1||opcode==13)
        return TRUE;
    else if(addressingMethod==IMMEDIATE)
    {
        printError(lineNum, "The addressing method of the destination operand cannot be immediate");
        return FALSE;
    }
    return TRUE;

}

int addOperands(Operation *poperation,Command command,char *firstOperand, char *secondOperand, int lineNum)
{
    int o1AddrssMethod, o2AddrssMethod, num=1, j, i, rgstr;
    char  labelTemp[N]={0}, *plableTmp=labelTemp;
    unsigned long decoded=(1<<18)+(command.func<<12); /* for the first A and the funct*/
    if(firstOperand)
    {
        o1AddrssMethod = addressingMethod(firstOperand);
        if(isFirstOperandLegal(o1AddrssMethod,command.opcode,lineNum,firstOperand)==FALSE)
            return ERROR;
        switch (o1AddrssMethod) {
            case IMMEDIATE:
                firstOperand++;
                if (firstOperand[0] == '-') {
                    num *= -1;
                    firstOperand++;
                }
                if (firstOperand[0] == '+')
                    firstOperand++;
                if(isANumber(firstOperand) == FALSE)
                {
                    printError(lineNum,"Unexpected character after declaring a number");
                    return ERROR;
                }
                num *= atoi(firstOperand);
                if (num > MAX_NUM || num < MIN_NUM) {
                    printError(lineNum,"Number out of the range");
                    return ERROR;
                }
                if(num>=0)
                    poperation->lineDecoded[poperation->lines]=num+(1<<18);
                else
                {
                    num&=MASK;
                    poperation->lineDecoded[poperation->lines]=(num&~(3<<16))+(1<<18);
                }
                poperation->lines+=1; /*the direct decoding method needs one line*/
                break;
            case REGISTER:
                decoded += (3 << 6);
                for (i = 0; i < 16; i++) {
                    if (!strcmp(firstOperand, registers[i]))
                    {
                        decoded += ((i) << 8);
                        break;
                    }
                }
                break;
            case DIRECT:
                decoded += (1 << 6);
                poperation->SourceLineNum = poperation->lines;
                poperation->lines+=2;/* the direct method needs 2 lines */
                poperation->sourceLabel = (char *) malloc(sizeof(char) * strlen(firstOperand));
                if (!poperation->sourceLabel)
                {
                    printf("ERROR: On allocating memory.\n");
                    exit(-1);
                }
                strcpy(poperation->sourceLabel, firstOperand);
                break;
            case INDEX:
                decoded += (2 << 6);
                poperation->SourceLineNum = poperation->lines; /* the line num in the matrix */
                poperation->lines+=2; /* the index method needs 2 lines */
                rgstr = extractRegister(firstOperand, plableTmp, lineNum);
                if (rgstr == ERROR)
                    return ERROR;
                j=0;
                while (firstOperand[j] != '[')
                {
                    j++;
                }

                decoded += (rgstr << 8);
                /*   poperation->sourceLabel = (char *) malloc(sizeof(char) * strlen(label));
                   if (!poperation->sourceLabel)
                   {
                       printf("ERROR: On allocating memory.\n");
                       exit(-1);
                   }
                   strcpy(poperation->sourceLabel, label);*/
                poperation->sourceLabel = (char*)malloc(sizeof(char)*j);
                if (!poperation->sourceLabel)
                {
                    printf("ERROR: On allocating memory.\n");
                    exit(-1);
                }
                j=0;
                while (firstOperand[j] != '[')
                {
                    poperation->sourceLabel[j]=firstOperand[j];
                    j++;
                }
                poperation->sourceLabel[j]='\0';
                break;
        }
    }

    o2AddrssMethod = addressingMethod(secondOperand);
    if(isSecondOperandLegal(o2AddrssMethod,command.opcode,lineNum,secondOperand)==FALSE)
        return ERROR;
    num=1;
    switch (o2AddrssMethod) {
        case IMMEDIATE:
            secondOperand++; /*for cutting the '#' */
            if(secondOperand[0]=='-')
            {
                num*=-1;
                secondOperand++; /*for cutting the '-' */
            }
            if(secondOperand[0]=='+')
                secondOperand++;
            if(isANumber(secondOperand) == FALSE)
            {
                printError(lineNum,"Unexpected character after declaring a number");
                return ERROR;
            }
            num*=atoi(secondOperand);
            if(num>MAX_NUM||num<MIN_NUM)
            {
                printError(lineNum,"Number out of the range");
                return ERROR;
            }
            if(num>=0)
                poperation->lineDecoded[poperation->lines]=num+(1<<18);
            else
            {
                num&=MASK;
                poperation->lineDecoded[poperation->lines]=(num&~(3<<16))+(1<<18);
            }
            poperation->lines++; /*the direct decoding method needs one line*/
            break;
        case REGISTER:
            decoded+=3;
            for (i = 0; i < 16; i++)
            {
                if (!strcmp(secondOperand,registers[i]))
                {
                    decoded+=(i<<2);
                    break;
                }
            }
            break;
        case DIRECT:
            decoded+=1;
            poperation->destinationLineNum=poperation->lines; /* the line num in the matrix */
            poperation->lines+=2;
            poperation->destinationLabel = (char*)malloc(sizeof(char)*strlen(secondOperand));
            if (!poperation->destinationLabel)
            {
                printf("ERROR: On allocating memory.\n");
                exit(-1);
            }
            strcpy(poperation->destinationLabel,secondOperand);
            break;
        case INDEX:
            j=0;
            decoded+=2;
            poperation->destinationLineNum=poperation->lines; /* the line num in the matrix */
            poperation->lines+=2;
            rgstr=extractRegister(secondOperand,plableTmp,lineNum);
            if(rgstr==ERROR)
                return ERROR;
            while (secondOperand[j] != '[')
            {
                j++;
            }
            decoded+=(rgstr<<2);
            poperation->destinationLabel = (char*)malloc(sizeof(char)*j);
            if (!poperation->destinationLabel)
            {
                printf("ERROR: On allocating memory.\n");
                exit(-1);
            }
            j=0;
            while (secondOperand[j] != '[')
            {
                poperation->destinationLabel[j]=secondOperand[j];
                j++;
            }
            poperation->destinationLabel[j]='\0';
            /* strcpy(poperation->destinationLabel,plableTmp);
            */ break;
    }
    /*insert the second line to the operation struct */
    poperation->lineDecoded[1]=decoded;
    return  TRUE;
}

int extractRegister(char *operand,char *label,int lineNum)
{
    char  rgstr[N]={0};
    int i = 0,j=0;
    while (operand[i] != '[')
    {
        if(isspace(operand[i]))
        {
            printError(lineNum,"Contains white space in a forbidden place");
            return ERROR;
        }
        if(operand[i]==']')
        {
            printError(lineNum,"Missing [");
            return ERROR;
        }
        i++;
    }
    /* label=(char*)malloc(sizeof(char)*strlen(labelTmp));
     if (!label)
     {
         printf("ERROR: On allocating memory.\n");
         exit(-1);
     }
     strcpy(label, labelTmp);*/
    i++;
    while (operand[i]&&operand[i] != ']')
    {
        if(isspace(operand[i]))
        {
            printError(lineNum,"Contains white space in a forbidden place");
            return ERROR;
        }
        if(operand[i]=='[')
        {
            printError(lineNum,"Contains extra [");
            return ERROR;
        }
        rgstr[j]=operand[i];
        i++;
        j++;
    }
    for (i = 0; i < 16; i++)
        if (!strcmp(rgstr,registers[i]))
            return i;
    return ERROR;
}

int addressingMethod(char *operand)
{
    int i=0;
    if(*operand == '#') {
        return IMMEDIATE;
    }
    for (i = 0; i < 16; i++)
    {
        if (!strcmp(operand,registers[i]))
            return REGISTER;
    }
    if(strchr(operand,'['))
        return INDEX;
    if(strchr(operand,']'))
        return INDEX;
    return DIRECT;
}

int isBlankLine(char* line)
{
    deleteWhiteSpace(line);
    return !line[0]? TRUE:FALSE;
}

int isLabel (char* line)
{
    char label[N]={0};
    int i=0;
    deleteWhiteSpace(line);
    while (line[i]&&!isspace(line[i]))
    {
        label[i]=line[i];
        i++;
    }
    if(label[strlen(label)-1]==':')
        return TRUE;
    return FALSE;
}

void extractLabel(Operation *poperation,char *line, int lineNum)
{
    int i=0;
    char label[N]={0};
    deleteWhiteSpace(line);
    while (line[i]&&!isspace(line[i]))
    {
        label[i]=line[i];
        i++;
    }
    memmove(line, line+strlen(label)+1, strlen(line));
    deleteWhiteSpace(line);
    if(isLegalLabel(label, lineNum,poperation)==TRUE)
    {
        poperation->label= (char*)malloc(sizeof(char)*strlen(label));
        if (!poperation->label)
        {
            printf("ERROR: On allocating memory.\n");
            exit(-1);
        }
        label[strlen(label)]='\0'; /*for cutting the ':*/
        strcpy(poperation->label, label);
    }
}

void deleteWhiteSpace(char* line)
{
    int i=0;
    while (line[i]&&isspace(line[i]))
        i++;
    memmove(line, line+i, strlen(line));
}

int isComment (char* line)
{
    deleteWhiteSpace(line);
    if(line[0] == ';')
        return TRUE;
    return FALSE;
}

int isDirectives (char* line)
{
    if(line[0] == '.')
        return TRUE;
    return FALSE;
}

int directiveType (char* line)
{
    char directive[N]={0};
    int i=0;
    while(line[i]&&!isspace(line[i]))
    {
        directive[i]=line[i];
        i++;
    }
    if(!strcmp(directive,".data"))
        return DATA;
    else  if(!strcmp(directive,".string"))
        return STRING;
    else if(!strcmp(directive,".entry"))
        return ENTRY;
    else if(!strcmp(directive,".extern"))
        return EXTERN;
    return ERROR;
}

int countComma(char *line)
{
    int i=0,count=0;
    for(;line[i];i++)
    {
        if(line[i]==',')
            count++;
    }
    return count;
}

void dataDecoding(Operation *poperation,char *line, int lineNum)
{
    int num,count=0; /*counts the number of numbers in data*/
    count=countComma(line)+1;
    poperation->dataDecoded=(long *)malloc(count*sizeof(long ));
    if (!poperation->dataDecoded)
    {
        printf("ERROR: On allocating memory.\n");
        exit(-1);
    }
    poperation->lines=1;
    poperation->directiveDestiny = DATA;
    memmove(line, line+ strlen(".data"), strlen(line));
    deleteWhiteSpace(line);
    if(lineIsEmpty(line)==TRUE)
    {
        printError(lineNum,"Missing definition after directive statement");
        return;
    }
    num = extractNum(line,lineNum);
    count++;
    if (num == ERROR)
        return;
    else
    {
        int i=1;
        if(num>=0)
            poperation->dataDecoded[0]=num+(1<<18);
        else
        {
            num&=MASK;
            poperation->dataDecoded[0]=(num&~(3<<16))+(1<<18);
        }
        while (isAnotherNum(line) == TRUE)
        {
            count++;
            if(isLegalComma (line)==FALSE)
            {
                printError(lineNum,"Missing comma");
                return;
            }
            strcpy(line,strchr(line,',')+1); /*delete the comma and the white space before if exist*/
            deleteWhiteSpace(line);
            num = extractNum(line,lineNum);
            if (num == ERROR)
                return;
            if(num>=0)
                poperation->dataDecoded[i]=num+(1<<18);
            else
            {
                num&=MASK;
                poperation->dataDecoded[i]=(num&~(3<<16))+(1<<18);
            }
            poperation->lines++;
            i++;
        }
        if (lineIsEmpty(line)==FALSE)
        {
            printError(lineNum,"Contains unnecessary characters at the end of a line");
            return;
        }
        poperation->isDecoded=TRUE;
    }
}

void srtringDecoding(Operation *poperation,char *line, int lineNum)
{
    int i=0, count;
    poperation->lines=0;
    poperation->directiveDestiny = STRING;
    strcpy(line, line+ strlen(".string"));
    deleteWhiteSpace(line);
    if(!strchr(line,'\"'))
    {
        printError(lineNum,"Missing left quotation mark");
        return;
    }
    if(strrchr(line,'\"')==strchr(line,'\"'))
    {
        printError(lineNum,"Missing quotation marks");
        return;
    }
    count=strrchr(line,'\"')-strchr(line,'\"')+1;
    poperation->dataDecoded=(long *)malloc(sizeof(long)*count);
    if (!poperation->dataDecoded)
    {
        printf("ERROR: On allocating memory.\n");
        exit(-1);
    }
    if(lineIsEmpty(line)==TRUE)
    {
        printError(lineNum,"Missing definition after directive statement");
        return;
    }
    if(!line[0]||line[0]!='\"')
    {
        printError(lineNum,"Missing left quotation mark");
        return;
    }
    line++; /*cutting the used character: '"'*/
    while (line[0]&&line[0]!='\"')
    {
        poperation->dataDecoded[i]=line[0]+(1<<18);
        poperation->lines++;
        line++; /*cutting the used character: '"'*/
        i++;
    }
    if(line[0]!='\"')
    {
        printError(lineNum,"Missing right quotation mark");
        return;
    }
    line++; /*cutting the used character: '"'*/
    if (lineIsEmpty(line)==FALSE)
    {
        printError(lineNum,"Contains unnecessary characters at the end of a line");
        poperation->isDecoded=FALSE;
    }
    /*for the last 0 at the end of string*/
    poperation->dataDecoded[i]=(1<<18);
    poperation->lines=i+1;
    poperation->isDecoded=TRUE;
}

void entryDecoding(Operation *poperation,char *line, int lineNum)
{
    poperation->directiveDestiny= ENTRY;
    memmove(line, line+ strlen(".entry"), strlen(line));
    deleteWhiteSpace(line);
    deleteWhiteSpace(line);
    if(lineIsEmpty(line)==TRUE)
    {
        printError(lineNum,"Missing definition after directive statement");
        return;
    }
    if(isLegalLabelEntry(line, lineNum,poperation)==TRUE);
    return;
}

void externDecoding(Operation *poperation,char *line, int lineNum)
{
    poperation->directiveDestiny= EXTERN;
    strcpy(line, line+ strlen(".extern"));
    deleteWhiteSpace(line);
    if(lineIsEmpty(line)==TRUE)
    {
        printError(lineNum,"Missing definition after directive statement");
        return;
    }
    if(isLegalLabelEntry(line, lineNum,poperation)==TRUE)
    {
        poperation->isDecoded=TRUE;
    }
    return;
}

int isLegalLabel(char *label, int lineNum,Operation *poperation)
{
    int i=0;
    deleteWhiteSpace(label);
    if(!isalpha(label[0]))
    {
        printError(lineNum,"Invalid label name, must begin with a letter");
        return FALSE;
    }
    while (label[i])
    {
        if(!(isalpha(label[i])|| isdigit(label[i])))
            if(i== strlen(label)-1 && label[i]!=':')
                return FALSE;
        i++;
    }
    label[strlen(label)-1]='\0'; /*cutting the : */
    for (i=0;i<38;i++)
    {
        if(!strcmp(label,keyWords[i]))
        {
            printError(lineNum,"A label name cannot be a keyword");
            return FALSE;
        }
    }
    return TRUE;
}

int extractNum(char *lineTmp, int lineNum)
{
    int i=0;
    char numInString[N]={0};
    int num = 1;
    if(lineTmp[0]==','||lineTmp[0]=='\n')
    {

        printError(lineNum,"Unexpected comma");
        return ERROR;
    }
    if(lineTmp[0]=='-')
    {
        num*=-1;
        memmove(lineTmp, lineTmp+1, strlen(lineTmp));
    }
    if(lineTmp[0]=='+')
        memmove(lineTmp, lineTmp+1, strlen(lineTmp));
    while (isdigit(lineTmp[i]))
    {
        numInString[i]=lineTmp[i];
        i++;
    }
    if (lineTmp[i]&&lineTmp[i]!=','&&!isspace(lineTmp[i]))
    {
        printError(lineNum,"Unidentified number");
        return ERROR;
    }
    memmove(lineTmp, lineTmp+strlen(numInString), strlen(lineTmp));
    if(atoi(numInString)==0&&numInString[0]!='0')
    {
        printError(lineNum,"Unexpected character after declaring a number");
        return ERROR;
    }
    num*=atoi(numInString);
    if(num>MAX_NUM||num<MIN_NUM)
    {
        printError(lineNum,"Number out of the range");
        return ERROR;
    }
    lineTmp+= strlen(numInString);
    return num;
}

int isAnotherNum(char *lineTmp)
{
    return strchr(lineTmp,',')?  TRUE :  FALSE;
}

int isLegalComma(char *lineTmp)
{
    deleteWhiteSpace(lineTmp);
    return ((lineTmp[0]==','))? TRUE : FALSE;
}

int lineIsEmpty(char *lineTmp)
{
    deleteWhiteSpace(lineTmp);
    return lineTmp[0]? FALSE : TRUE;
}

int isLegalLabelEntry(char *lineTmp, int lineNum,Operation *poperation)
{
    int i=0;
    char label[N]={0};
    deleteWhiteSpace(lineTmp);
    if(!lineTmp[0])
    {
        printError(lineNum,"Missing label name after  entry or external definition");
        return FALSE;
    }
    if(!isalpha(lineTmp[0]))
    {
        printError(lineNum,"Invalid label name, must begin with a letter");
        return FALSE;
    }
    while (lineTmp[i]&&(isalpha(lineTmp[i])|| isdigit(lineTmp[i])))
    {
        label[i]=lineTmp[i];
        i++;
    }

    if(lineTmp[i]&&(!isspace(lineTmp[i])))
    {
        printError(lineNum,"Invalid label - contains an invalid character");
        return FALSE;
    }
    for (i=0;i<38;i++)
    {
        if(!strcmp(label,keyWords[i]))
        {
            printError(lineNum,"A label name cannot be a keyword");
            return FALSE;
        }

    }
    memmove(lineTmp, lineTmp+strlen(label), strlen(lineTmp));
    if (lineIsEmpty(lineTmp)==FALSE)
    {
        printError(lineNum,"Contains unnecessary characters at the end of a line");
        return FALSE;
    }
    poperation->label= (char*)malloc(sizeof(char)*strlen(label));
    if (!poperation->label)
    {
        printf("ERROR: On allocating memory.\n");
        exit(-1);
    }
    strcpy(poperation->label, label);
    return TRUE;
}

int isANumber(char* number)
{
    int i = 0;
    while(number[i])
    {
        if(!isdigit(number[i]))
        {
            return FALSE;
        }
        i++;
    }
    return TRUE;
}