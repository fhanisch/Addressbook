/*
__________________________________________________________________________________

	gcc -o Addressbook main.c `pkg-config gtk+-3.0 --cflags --libs`
__________________________________________________________________________________
*/

#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <socket_comm.h>


#define FILENAME "book1.adr"


typedef struct{
	char *name;
	char *preName;
	char *age;
	char *phone;
} Addr;

static unsigned int entries;
static int active_id=-1;
static GtkEntry *eName, *ePreName, *eAge, *ePhone;
static GtkComboBoxText *addrList;


static void addAddr(Addr **book, Addr addr)
{
	Addr *tmpBook;
	//char *ptr;

	tmpBook = *book;
	*book = calloc(entries+1,sizeof(Addr));	
	memcpy(*book,tmpBook,entries*sizeof(Addr));
	memcpy(*book+entries,&addr,sizeof(Addr)); //Adressindizierung über Pointer-Typ
	/* alternativ auch das möglich:
	ptr=(char*)*book;
	memcpy(ptr+entries*sizeof(Addr),&addr,sizeof(Addr));
	*/	
	free(tmpBook);
}

void cpyEntry(char **str, GtkEntry *e)
{
	char *text = g_strdup(gtk_entry_get_text(e));
	unsigned int len = strlen(text)+1; 
	*str = malloc(len);	
	strcpy(*str,text);	
}

static void saveAddr(GtkWidget *widget, Addr **book)
{
	Addr addr;
	FILE *file;
	char strbuf[128];
	unsigned int i;
	
	cpyEntry(&addr.name, eName);
	cpyEntry(&addr.preName, ePreName);
	cpyEntry(&addr.age, eAge);
	cpyEntry(&addr.phone, ePhone);

	g_print("Speichere Adresse\n");
	g_print("Name: %s\n",addr.name);
	g_print("Vorname: %s\n",addr.preName);
	g_print("Alter: %s\n",addr.age);
	g_print("Telefon: %s\n",addr.phone);
	
	if (active_id==-1)
	{				
		addAddr(book, addr);
		sprintf(strbuf,"%d",entries);
		gtk_combo_box_text_append(addrList,strbuf,addr.name);
		gtk_combo_box_set_active_id((GtkComboBox*)addrList,strbuf);
		entries++;
	}
	else
	{		
		memcpy(*book+active_id,&addr,sizeof(Addr));
	}

	file = fopen(FILENAME,"w");
	fwrite(&entries,1,sizeof(entries),file);
	for (i=0;i<entries;i++)
	{
		fwrite((*book)[i].name,1,strlen((*book)[i].name)+1,file);
		fwrite((*book)[i].preName,1,strlen((*book)[i].preName)+1,file);
		fwrite((*book)[i].age,1,strlen((*book)[i].age)+1,file);
		fwrite((*book)[i].phone,1,strlen((*book)[i].phone)+1,file);
	}
	fclose(file);		
}

static void setAddr(GtkWidget *widget, Addr **book)
{
	char *str;
	
	str = gtk_combo_box_text_get_active_text(addrList);
	g_print("%s\n",str);
	str = g_strdup(gtk_combo_box_get_active_id((GtkComboBox*)addrList));
	g_print("%s\n",str);	
	sscanf(str,"%d",&active_id);
	if (active_id==-1)
	{
		gtk_entry_set_text(eName,"");
		gtk_entry_set_text(ePreName,"");
		gtk_entry_set_text(eAge,"");
		gtk_entry_set_text(ePhone,"");		
	}
	else
	{		
		gtk_entry_set_text(eName,(*book)[active_id].name);
		gtk_entry_set_text(ePreName,(*book)[active_id].preName);		
		gtk_entry_set_text(eAge,(*book)[active_id].age);
		gtk_entry_set_text(ePhone,(*book)[active_id].phone);
	}
	free(str);
}

int main(int argc, char **argv)
{
	GtkBuilder *builder;
	GObject *window;
	GtkButton *btnSave;	
	FILE *file;
	unsigned int filesize;
	Addr *book;
	char **field;
	unsigned int i,j,k;
	char strbuf[128];
	char c;	
	int port=12345;
	char ip[]="192.168.1.100";		
	int status;

	if(argc>1)
	{
		if(!strcmp(argv[1],"-v"))
		{
			printf("*** Adressbuch ***   Author: FH   Version 0.1   19.11.2016\n");
			return 0;
		}
		else
		{
			printf("Unknown command!\n");
			return 0;
		}
	}
		
	status = createClient(port, ip);
	if(status)
	{
		printf("%s",getLastErr());
		return -1;
	}
	
	status = sendCmdToServer(CMD_OPEN_FILE);
 
	file = fopen(FILENAME,"r");
	if (!file) 
	{
		entries=0;
		book = malloc(sizeof(Addr));
		printf("Keine Datei!\n");
	}
	else
	{
		fseek(file,0,SEEK_END);
		filesize=ftell(file);
		rewind(file);
		fread(&entries,1,sizeof(entries),file);
		book = malloc(entries*sizeof(Addr));		
		for (i=0;i<entries;i++)
		{
			field = (char**)&book[i];
			for (j=0;j<4;j++)
			{				
				k=0;
				c=1;		
				while (c!=0)
				{
					fread(strbuf+k,1,1,file);
					c=strbuf[k];
					k++;
				}
				field[j] = malloc(k);
				strcpy(field[j],strbuf);				
				printf("%s\n",field[j]);				
			}			
		}	
		fclose(file);
		printf("Datei %s geladen.\n",FILENAME);
		printf("Dateigröße: %d\n",filesize);
		printf("Einträge: %d\n",entries);
		printf("Size of Address: %d\n",sizeof(Addr));
	}

	gtk_init (&argc, &argv);

	builder = gtk_builder_new ();
	gtk_builder_add_from_file (builder, "main_gui.glade", NULL);

	window = gtk_builder_get_object(builder, "window");	
	eName = (GtkEntry*)gtk_builder_get_object(builder, "eName");
	ePreName = (GtkEntry*)gtk_builder_get_object(builder, "ePreName");
	eAge = (GtkEntry*)gtk_builder_get_object(builder, "eAge");
	ePhone = (GtkEntry*)gtk_builder_get_object(builder, "ePhone");
	btnSave = (GtkButton*)gtk_builder_get_object(builder, "btnSave");	
	addrList = (GtkComboBoxText*)gtk_builder_get_object(builder, "addrList");

	sprintf(strbuf,"%d",-1);
	gtk_combo_box_text_append(addrList,strbuf,"(neu)");
	gtk_combo_box_set_active_id((GtkComboBox*)addrList,strbuf);
	for (i=0;i<entries;i++)
	{
		sprintf(strbuf,"%d",i);
		gtk_combo_box_text_append(addrList,strbuf,book[i].name);
	}	
	
	g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
	g_signal_connect (btnSave, "clicked", G_CALLBACK (saveAddr), &book);	
	g_signal_connect (addrList, "changed", G_CALLBACK (setAddr), &book);

	gtk_main ();

	return 0;
}
