#include "AprilTagReader.h"



int main(int argc, char *argv[])
{
    AprilTagReader reader (735.115, 0, 640, 480, 319.5, 239.5, 16.5);

    // Récupération de la 1ere image
    cv::Mat image = reader.getImage();


    while(true)
    {
        // cv::waitKey(1) permet d'afficher les fenetres. Renvoie une valeur négative si on essaye de quitter la fenetre
        if (cv::waitKey(1) != 255)
            break;
        image = reader.getImage();
        // Fonction cv::imshow("NomDeLaFenetre", image) pour afficher une image dans une fenetre
        imshow("Tag Detections", image);

        //reader.getTags();
    }

    return 0;
}



