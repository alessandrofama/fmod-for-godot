# Original version: https://github.com/utopia-rise/fmod-gdnative/blob/godot-4.0-adaption/get_fmod.py
# Modified to include Studio download for Windows, macOS, Linux and API download for html5.

# MIT License

# Copyright (c) 2019 Utopia-Rise and Alex Fonseka

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

import requests
import sys
import argparse

parser = argparse.ArgumentParser(description="FMOD Downloader")
parser.add_argument("user", help="FMOD username")
parser.add_argument("password", help="FMOD account password")
parser.add_argument("type", help="api or studio")
parser.add_argument(
    "platform",
    help="Platform to download for (windows, mac, linux, android, ios, html5)",
)
parser.add_argument("fmod_version", help="FMOD version to download (i.e. 20211)")
args = parser.parse_args()

user = args.user
password = args.password
type = args.type
platform = args.platform
fmod_version = args.fmod_version

if type != "api" and type != "studio":
    print("Invalid download type. Choose from 'api' or 'studio'")
    sys.exit(1)

fmodlink = "https://www.fmod.com/api-login"

if platform == "windows":
    if type == "api":
        filename = f"fmodstudioapi{fmod_version}win-installer.exe"
        downloadlink = f"https://www.fmod.com/api-get-download-link?path=files/fmodstudio/api/Windows/&filename={filename}&user_id="
    elif type == "studio":
        filename = f"fmodstudio{fmod_version}win64-installer.exe"
        downloadlink = f"https://www.fmod.com/api-get-download-link?path=files/fmodstudio/tool/Win64/&filename={filename}&user_id="
elif platform == "mac":
    if type == "api":
        filename = f"fmodstudioapi{fmod_version}mac-installer.dmg"
        downloadlink = f"https://www.fmod.com/api-get-download-link?path=files/fmodstudio/api/Mac/&filename={filename}&user_id="
    elif type == "studio":
        filename = f"fmodstudio{fmod_version}mac-installer.dmg"
        downloadlink = f"https://www.fmod.com/api-get-download-link?path=files%2Ffmodstudio%2Ftool%2FMac%2F&filename={filename}&user_id="
elif platform == "linux":
    if type == "api":
        filename = f"fmodstudioapi{fmod_version}linux.tar.gz"
        downloadlink = f"https://www.fmod.com/api-get-download-link?path=files/fmodstudio/api/Linux/&filename={filename}&user_id="
    elif type == "studio":
        filename = f"fmodstudio{fmod_version}linux64-installer.AppImage"
        downloadlink = f"https://www.fmod.com/api-get-download-link?path=files/fmodstudio/tool/Linux/&filename={filename}&user_id="
elif platform == "android":
    filename = f"fmodstudioapi{fmod_version}android.tar.gz"
    downloadlink = f"https://www.fmod.com/api-get-download-link?path=files/fmodstudio/api/Android/&filename={filename}&user_id="
elif platform == "ios":
    filename = f"fmodstudioapi{fmod_version}ios-installer.dmg"
    downloadlink = f"https://www.fmod.com/api-get-download-link?path=files/fmodstudio/api/iOS/&filename={filename}&user_id="
elif platform == "html5":
    filename = f"fmodstudioapi{fmod_version}html5.zip"
    downloadlink = f"https://www.fmod.com/api-get-download-link?path=files/fmodstudio/api/HTML5/&filename={filename}&user_id="
else:
    print("Invalid platform. Choose from (windows, mac, linux, android, ios, html5)")
    sys.exit(1)

downloadlink += user

try:
    response = requests.post(fmodlink, auth=(user, password)).json()
    token = response["token"]
    print("Received token from FMOD login API.")

    response = requests.get(
        downloadlink, headers={"Authorization": f"Bearer {token}"}
    ).json()
    url = response["url"]

    response = requests.get(url, allow_redirects=True)
    if response.status_code != 200:
        print(f"Error: Failed to download {filename}, status code: {response.status_code}")
        sys.exit(1)
    open(filename, "wb").write(response.content)
    print("Downloading FMOD using the requested download link.")
except Exception as e:
    print("An error occurred: ", e)
    sys.exit(1)
else:
    print("FMOD download completed successfully.")