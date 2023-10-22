import cv2
from PIL import Image
from .html_generator import html_msg

def capture_webcam_image(default_value=None):
    try:
        # initialize the camera
        camera = cv2.VideoCapture(0, cv2.CAP_DSHOW)

        # check if camera is opened successfully
        if not camera.isOpened():
            raise Exception("Unable to open camera")

        # capture a frame from the camera
        bool, image = camera.read()

        if bool:
            # convert color space from BGR to RGB
            rgb_frame = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)

            # create a PIL Image from the numpy array
            pil_image = Image.fromarray(rgb_frame)

            return html_msg('The webcam capture is successful.', True, bold_all=True), pil_image
        else:
            raise Exception("Unable to capture")
    except Exception as e:
        error_msg = f"Error: {str(e)}"
        return html_msg(error_msg, False, bold_all=True), None
