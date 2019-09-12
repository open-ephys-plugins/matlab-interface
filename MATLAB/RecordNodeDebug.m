ROOT = 'C:\Users\pavel.kulik\Projects\plugin-GUI\Build\Release\';
FILE = '\experiment-1\recording1\';

NIDAQ = '2019-09-12_14-53-42';
NPX1 = '2019-09-12_14-54-31';
NPX2 = '2019-09-12_14-55-02';

NIDAQ = strcat(ROOT, NIDAQ, FILE);
NPX1 = strcat(ROOT, NPX1, FILE);
NPX2 = strcat(ROOT, NPX2, FILE);

%FileReader(NIDAQ);
FileReader(NPX1);
%FileReader(NPX2);