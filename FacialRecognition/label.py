import imghdr
import os,sys
from PIL import Image
import PIL
import face_recognition
import cv2

neutre = 0
content = 0
colere = 0
triste = 0

def print_file(name,text):
    fichier = open("../dataset/"+name+".txt", "w")
    fichier.write(''.join(text))
    fichier.close()

    
def resize(imgPath, name):
    basewidth = 448
    img = Image.open(imgPath)
    wpercent = (basewidth / float(img.size[0]))
    hsize = int((float(img.size[1]) * float(wpercent)))
    img = img.resize((basewidth, hsize), PIL.Image.ANTIALIAS)
    img.save("../dataset/"+name)

def labelTxt(imgName, label, top, left, bottom, right, img_width, img_height):
    x_center = (left+right)/2
    x_center = str(x_center/img_width)
    y_center = (top+bottom)/2
    y_center = str(y_center/img_height)
    width = str(abs(right-left)/img_width)
    height = str(abs(top-bottom)/img_height)
    imgName = (imgName.split('.'))[0]
    print_file(imgName,(label," ",x_center," ",y_center," ",width," ",height))
    #label x_center y_center width height
    
def label(path):
    global neutre
    global content
    global colere
    global triste
    files = os.listdir(path) #On récupère le dossier courant
    for name in files: #Pour chaque fichier
        if not(os.path.isdir(path+'/'+name)) and imghdr.what(path+'/'+name): #Si le fichier est bien une image
            #On verifie si le nom du fichier est dans le fichier en csv
            try:
                res = os.popen("cat training.csv | grep \""+name+";[0-9]*\"").read() #On récupère la ligne dans le fichier csv
                res1 = res.split(";")
                res2 = res1[0].split("/")
                res1[1] #pour tester s'il existe
                res2[1] #idem sinon => except => break
            except:
                break;
            ##Si oui alors on récupère le label
            if res1[1] == "0":
                #print("neutre")
                neutre += 1
            elif res1[1] == "1":
                #print("content")
                content += 1
            elif res1[1] == "2":
                #print("triste")
                triste += 1
            elif res1[1] == "6":
                res1[1] = "3"
                #print("colere")
                colere += 1
            else:
                continue
            print(res2[1]) #nom du fichier
            print(os.path.getsize(path+"/"+res2[1]))
            if os.path.getsize(path+"/"+res2[1]) > 250000:
                continue
            #print(res1[1]) #label
            ##On execute Yolo sur l'image
            image = face_recognition.load_image_file(path+"/"+res2[1])
            face_locations = face_recognition.face_locations(image, number_of_times_to_upsample=0, model="cnn")
            ##On récupère la sortie (et on l'affiche)
            for face_location in face_locations:
                top, right, bottom, left = face_location
                #print("A face is located at pixel location Top: {}, Left: {}, Bottom: {}, Right: {}".format(top, left, bottom, right))
                #face_image = image[top:bottom, left:right] affichage du visage
                #pil_image = Image.fromarray(face_image) affichage du visage
                #pil_image.show() affichage du visage
                img = cv2.imread(path+"/"+res2[1],0)
                height, width = img.shape[:2] #on récupère les dimensions de l'image
                ##On redimentionne l'image et on la sauvegarde dans le dossier dataset
                resize(path+"/"+res2[1],res2[1])
                ##On écrit dans le fichier txt dans dataset
                labelTxt(res2[1],res1[1],top,left,bottom,right,width,height)
        elif os.path.isdir(name):
            label(path+'/'+name)
   
label('.')
print('')
print("Nb neutre : " , neutre)
print("Nb content : " , content)
print("Nb triste : " , triste)
print("Nb colere : " , colere)
