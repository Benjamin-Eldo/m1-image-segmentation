#include "../headers/RegionFactory.hpp"
#include "../headers/ImageRegionGrowing.hpp"
#include <cmath>

RegionFactory::RegionFactory(){

}

/**
 * Remplit un tableau passé en paramètre avec des nombres choisis aléatoirement entre 0 et nbCandidats.  
 * Par exemple pour un tableau de taille 9, et un nbCandidats de 25, choisit 9 nombres aléatoires différents entre 0 et 24.  
 * @param chosenAreaIndexes Modifié en place, tableau qui contiendra les indices des régions choisies
 * @param nbRegion Taille du tableau = Nombre de régions souhaitées
 * @param nbCandidats Nombre d'indices possibles = Nombre de zones parmi lesquelles choisir  
*/
void fillArrayWithIndexes(int (&chosenAreaIndexes)[], int nbRegion, int nbCandidats) {
    // On seed l'aléa sur le temps à l'execution
    std::srand(std::time(0));

    int nbRegionChoisies = 0;
    while (nbRegionChoisies < nbRegion) {
        int regionIndex = std::rand()%nbCandidats;
        nbRegionChoisies++;
        bool uniqueRegion = false;
        while (!uniqueRegion) {
            uniqueRegion = true;
            regionIndex = std::rand()%nbCandidats;
            for (int i = 0; i < nbRegionChoisies; i++) {
                if (regionIndex == chosenAreaIndexes[i]) {
                    uniqueRegion = false;
                }
            }
        }
        chosenAreaIndexes[nbRegionChoisies-1] = regionIndex;
    }
}

std::vector<Region*> RegionFactory::buildRegions(cv::Mat image,int nbRegion){
    /**
     * Approche : divise la longueur/largeur image par |⁻nbRegion/2⁻| (Arrondi à l'entier le + proche) 
     * et choisit nbRegion parmi celles découpées ainsi
     * ex : nbRegion = 5 : découpe image en 9 zones et choisit 5 de ces 9 zones
     *      nbRegion = 6 : découpe image en 9 zones et choisit 6 de ces 9 zones
    */

    // nb de lignes & colonnes de la grille générée par la suite
    int nbColumnsRows = round(nbRegion/2.0);
    // nombre de régions candidates
    int nbGridAreas = nbColumnsRows*nbColumnsRows;
    // dimensions de chaque région candidate
    int regionWidths = image.cols/nbColumnsRows;
    int regionHeights = image.rows/nbColumnsRows;

    int chosenAreaIndexes[nbRegion];

    for (int i = 0; i < nbRegion; i++) {
        chosenAreaIndexes[i] = 0;
    }

    fillArrayWithIndexes(chosenAreaIndexes, nbRegion, nbGridAreas);

    std::vector<Region*> listeRegion;
    //cv::Mat* regions = new cv::Mat(image.rows, image.cols, CV_8UC3, cv::Scalar(0, 0, 0));
    std::vector<std::vector<short>>* regions = new std::vector<std::vector<short>>(image.rows, std::vector<short> (image.cols, -1)) ;
    for (int i = 0; i < nbRegion; i++) {
        int index = chosenAreaIndexes[i];
        // les coords X de la région i commencent à (i % nbColumnsRows)*regionWidths;
        // les coords Y de la région i commencent à floor(i / nbColumnsRows)*regionHeights;
        int debutX = (index % nbColumnsRows) * regionWidths;
        int debutY = (index / nbColumnsRows) * regionHeights;
        int x = (std::rand() % regionWidths) + debutX;
        int y = (std::rand() % regionHeights) + debutY;
        Pixel origine = Pixel(x,y);
        
        Region* region = new Region(image, regions, origine, (short) i);
        // Initialisation de la map index-->region*
        listeRegion.push_back(region);
    }

    return listeRegion;
}