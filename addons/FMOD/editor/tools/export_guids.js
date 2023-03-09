function writeAllGUIDs(textFile) {
    var allEvents = studio.project.model.Event.findInstances();
    var allBuses = studio.project.model.MixerGroup.findInstances();
    allBuses = allBuses.concat(studio.project.model.MixerReturn.findInstances());
    allBuses = allBuses.concat(studio.project.workspace.mixer.masterBus);
    var allVCAs = studio.project.model.MixerVCA.findInstances();
    var allSnapshots = studio.project.model.Snapshot.findInstances();
    var allBanks = studio.project.model.Bank.findInstances({
        includeDerivedTypes: true
    });
    var allParameters =  studio.project.model.ParameterPreset.findInstances();

    printGUID(allEvents, textFile, "Events");
    printGUID(allBuses, textFile, "Busses");
    printGUID(allVCAs, textFile, "VCAs");
    printGUID(allSnapshots, textFile, "Snapshots");
    printGUID(allBanks, textFile, "Banks");
    printGUID(allParameters, textFile, "Parameters");
}

function printGUID(managedObjects, textFile, managedObjectType) {
    if (managedObjects.length === 0) {
        return;
    }

    managedObjects.sort(function(a, b) {
        var pathA = a.getPath().toUpperCase();
        var pathB = b.getPath().toUpperCase();

        if (pathA < pathB) {
            return -1;
        }
        if (pathA > pathB) {
            return 1;
        }
        return 0;
    });

    textFile.writeText("class " + managedObjectType + ":\r\n");

    var whitespace = "";
    var identifierPrefix = "";
    var guidType = "";
    var guidConstructor = "";

    whitespace = "	";
    guidType = "const ";

    managedObjects.forEach(function(object) {
        if (object.isOfExactType("Event")) {
            if (object.banks.length === 0) {
                return;
            }
        }

            var identifier = identifierPrefix + identifierForObject(object);
            identifier = identifier.replace(/(^[0-9])/g, "_$1"); // Don't allow identifier to start with a number
    
            var guid = guidStringToStruct(object.id);
    
            identifier = identifier.toUpperCase()
            textFile.writeText(whitespace + guidType + identifier + " = " + guidConstructor + guid + "\r\n");

    });

    textFile.writeText("\r\n");
}


function identifierForObject(managedObject) {
    var path = managedObject.getPath();
    if (managedObject.isOfExactType("MixerMaster")) {
        path = managedObject.name;
    }

    path = path.replace(/^(.*):\/|[\(\)]/g, '');
    path = path.replace(/[^\w]/g, "_");
    return path;
}

function guidStringToStruct(guidString) {
    var guidContents = guidString.replace(/[{}-]/g, "");

    // for example {1f687138-e06c-40f5-9bac-57f84bbcedd3}
    var groupedGuid = [
        guidContents.substring(0, 8), // the first 8 hexadecimal digits of the GUID   - 1f687138
        guidContents.substring(8, 12), // the first group of 4 hexadecimal digits      - e06c
        guidContents.substring(12, 16), // the second group of 4 hexadecimal digits     - 40f5
        guidContents.substring(16, 32).match(/.{2}/g) // array of 8 bytes                             - [9b, ac, 57, f8, 4b, bc, ed, d3]
    ];

    function groupToStructure(group) {
        return group.map(function(item) {
            if (item instanceof Array) {

                return groupToStructure(item);

            } else {
                return '0x' + item;
            }
        }).join(", ");
    }

    return '"' + guidString + '"';
}

module.exports = {
    writeGuids: writeAllGUIDs,
    //this is main function
    writeHeaderFile: function(outputPath) {

        var headerFileName;
        var headerLanguage;

        headerFileName = outputPath.replace(/^.*[\\\/]/, '')
        headerLanguage = "GDScript header";

        var projectPath = studio.project.filePath;
        var projectName = projectPath.substr(projectPath.lastIndexOf("/") + 1, projectPath.length);

        var textFile = studio.system.getFile(outputPath);
        if (!textFile.open(studio.system.openMode.WriteOnly)) {
            alert("Failed to open file {0}\n\nCheck the file is not read-only.".format(outputPath));
            console.error("Failed to open file {0}.".format(outputPath));
            return;
        }

        textFile.writeText("#\r\n   # " + headerFileName + " - FMOD Studio API\r\n\r\n   # Generated GUIDs for project '" + projectName + "'\r\n#\r\n\r\n");
        textFile.writeText("class_name FMODGuids\r\n\r\n");
        module.exports.writeGuids(textFile);
        textFile.close();

        return "Header file successfully created at: " + outputPath;
    },
};