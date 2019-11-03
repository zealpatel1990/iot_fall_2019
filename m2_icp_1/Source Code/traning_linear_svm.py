import os
from keras.applications.vgg19 import VGG19
from keras.preprocessing import image
from keras.applications.vgg19 import preprocess_input
from keras.models import Model
import numpy as np

base_model = VGG19(weights='imagenet')
model = Model(inputs=base_model.input, outputs=base_model.get_layer('flatten').output)

def get_features(img_path):
    img = image.load_img(img_path, target_size=(224, 224))
    x = image.img_to_array(img)
    x = np.expand_dims(x, axis=0)
    x = preprocess_input(x)
    flatten = model.predict(x)
    return list(flatten[0])

X = []
y = []

cat_plots = []
for (_,_,filenames) in os.walk('catPlots'):
    cat_plots.extend(filenames)
    break

for cplot in cat_plots:
    X.append(get_features('catPlots/' + cplot))
    y.append(0)

dog_plots = []
for (_,_,filenames) in os.walk('dogPlots'):
    dog_plots.extend(filenames)
    break

for cplot in dog_plots:
    X.append(get_features('dogPlots/' + cplot))
    y.append(1)

from sklearn.model_selection import train_test_split
from sklearn.svm import LinearSVC
from sklearn.metrics import accuracy_score

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.10, random_state=42, stratify=y)

clf = LinearSVC(random_state=0, tol=1e-5)
clf.fit(X_train, y_train)
predicted = clf.predict(X_test)
print (predicted)
# get the accuracy
print (accuracy_score(y_test, predicted))
