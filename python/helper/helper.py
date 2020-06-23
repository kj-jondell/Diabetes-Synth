import csv
from PySide2.QtWidgets import QLabel, QWidget

#TODO change these to shared module
def read_settings(path):
    settings_dict = {}
    try:
        with open(path) as csvfile:
            reader = csv.reader(csvfile)
            settings_dict = {row[0]:row[1][1:] for row in reader}
    except FileNotFoundError:
        pass # ignore missing settings file!
    return settings_dict 

def write_settings(settings_dict, path):
    with open(path, 'w') as csvfile:
        writer = csv.writer(csvfile)
        for value in settings_dict:
            writer.writerow([value, " "+ settings_dict[value]])

# TODO group objects that are settings control instead of hardcoded hack !!!
def change_enabled_settings(widgets, exceptions = ["file_chooser"], enable = True):
    for child in widgets.children():
        if type(child) != QLabel and child.objectName() not in exceptions:
            try: 
                child.setEnabled(enable)
            except:
                pass

def list_to_string(input_list = []):
    return str(input_list)[1:-1].replace(',', '')

