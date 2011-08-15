#ifndef _TECHTREE_H_
#define _TECHTREE_H_

#include <string>
#include "JSON/JSON.h"
#include "imageManager.h"
#include "unit.h"
#include "building.h"

/**
 * Loads a set of Entity's and defines their attributes.
 *
 * A Player uses a TechTree to create its Units, Buildings and to upgrade them.
 */
class TechTree {
  public:
    /**
     * Constructs a TechTree from a resource file.
     *
     * @param filename    the name of the resource file
     * @param imgMgr      a pointer to the ImageManager from which Entity
     *                    Texture's are retrieved
     *
     * @return            a pointer to the constructed TechTree
     */
    static TechTree *fromFile(std::string filename, ImageManager *imgMgr);

    /**
     * Constructs a TechTree from a JSONObject.
     *
     * @param tree      the source JSONObject
     * @param imgMgr    a pointer to the ImageManager from which Entity
     *                  Texture's are retrieved
     *
     * @return          a pointer to the constructed TechTree
     */
    static TechTree *fromJSONObject(const JSONObject &tree, ImageManager *imgMgr);

    /**
     * Destructor.
     */
    ~TechTree();

    /**
     * Returns a pointer to the reference Entity for a given class.
     *
     * @param name    the className of the Entity
     *
     * @return        a pointer to the corresponding Entity
     */
    const Entity *getEntity(std::string name);

    /**
     * Creates a new Entity of a given class.
     *
     * @param name    the className of the Entity
     *
     * @return        a pointer to the created Entity
     */
    Entity *createEntity(std::string name);

  private:
    typedef std::map<std::string, const Unit*> UnitMap;
    typedef std::map<std::string, const Building*> BuildingMap;

    UnitMap units;
    BuildingMap buildings;

    TechTree();

    static std::string wstrToStr(const std::wstring &wstr);
};

#endif /* _TECHTREE_H_ */
