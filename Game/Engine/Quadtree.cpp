#include "QuadTree.h"
#include "../Drawer.h"
namespace thirtythree {

QuadTree::QuadTree(sf::FloatRect boundary, Drawer *drawer)
    : boundary_ (boundary),
      north_west_ (nullptr),
      north_east_ (nullptr),
      south_west_ (nullptr),
      south_east_ (nullptr),
      is_leaf_ (true),
      drawer_ (drawer) {
    objects_in_node_.reserve(node_capacity_);
}

QuadTree::~QuadTree() {
    if (north_west_ != nullptr) delete north_west_;
    if (north_east_ != nullptr) delete north_east_;
    if (south_west_ != nullptr) delete south_west_;
    if (south_east_ != nullptr) delete south_east_;
}


bool QuadTree::Insert(ObjectPtr object) {

    if (!boundary_.intersects(object->GetBoundary())) return false;

    if (is_leaf_) {
        if (objects_in_node_.size() < node_capacity_) {
            objects_in_node_.push_back(object);
            return true;
        } else {
            Subdivide();
        }
    }

    bool added_somewhere = north_west_->Insert(object) ||
                           north_east_->Insert(object) ||
                           south_west_->Insert(object) ||
                           south_east_->Insert(object);

    if (added_somewhere) return true;
    return false;
}

void QuadTree::Subdivide() {
    sf::FloatRect north_west(boundary_.left + boundary_.width / 2,
                             boundary_.top,
                             boundary_.width / 2,
                             boundary_.height / 2);
    north_west_ = new QuadTree(north_west, drawer_);
    sf::FloatRect north_east(boundary_.left,
                             boundary_.top,
                             boundary_.width / 2,
                             boundary_.height / 2);
    north_east_ = new QuadTree(north_east, drawer_);
    sf::FloatRect south_west(boundary_.left + boundary_.width / 2,
                             boundary_.top + boundary_.height / 2,
                             boundary_.width / 2,
                             boundary_.height / 2);
    south_west_ = new QuadTree(south_west, drawer_);
    sf::FloatRect south_east(boundary_.left,
                             boundary_.top + boundary_.height / 2,
                             boundary_.width / 2,
                             boundary_.height / 2);
    south_east_ = new QuadTree(south_east, drawer_);
    for (auto& obj : objects_in_node_) {
        north_west_->Insert(obj);
        north_east_->Insert(obj);
        south_west_->Insert(obj);
        south_east_->Insert(obj);
    }
    objects_in_node_.clear();
    is_leaf_ = false;
}

void QuadTree::Prune() {
    if (north_west_ != nullptr) delete north_west_;
    if (north_east_ != nullptr) delete north_east_;
    if (south_west_ != nullptr) delete south_west_;
    if (south_east_ != nullptr) delete south_east_;
    objects_in_node_.clear();
    is_leaf_ = true;
}

QuadTree::ObjectPtr QuadTree::FindNearestNeighbor(QuadTree::ObjectPtr object, float distance, bool visualise) {
    ObjectPtr nearest;
    std::stack<QuadTree*> trees;
    sf::Vector2f pos = object->GetPos();
    trees.push(this);
    sf::Color tree_color(0, 0, 0, 150);
    int number_leaves = 0;
    while (!trees.empty()) {
        sf::FloatRect search_area(pos.x - distance / 2, pos.y - distance / 2, distance, distance);
        auto tree = trees.top();
        if (!tree->IsLeaf()) {
            auto children = tree->GetChildren();
            trees.pop();
            for (auto& child : children) {
                if (child->GetBoundary().intersects(search_area)) {
                    trees.push(child);
                }
            }
        } else {
            if (tree->GetBoundary().intersects(search_area)) {
                if (object->GetType() == PLAYER && visualise == true) {
                    tree_color = tree_color + sf::Color(0, 0, 0, 5);
                    drawer_->VisualizeQuadTree(*tree, tree_color);
                }
                number_leaves++;
                std::vector<ObjectPtr> leaf_objs = tree->AccessObjects();
                for (auto& leaf_obj : leaf_objs) {
                    if (!leaf_obj->IsDead() && leaf_obj->IsInteractable()) {
                        auto leaf_obj_pos = leaf_obj->GetPos();
                        float new_distance = length(pos - leaf_obj_pos);
                        if (new_distance < distance && leaf_obj != object) {
                            nearest = leaf_obj;
                            distance = new_distance;
                        }
                    }
                }
            }
            trees.pop();
        }
    }
    return nearest;
}

int QuadTree::GetLeavesNumber() {
    int count = 0;
    std::stack<QuadTree*> trees;
    trees.push(this);
    while (!trees.empty()) {
        auto tree = trees.top();
        if (!tree->IsLeaf()) {
            auto children = tree->GetChildren();
            trees.pop();
            for (auto& child : children) {
                trees.push(child);
            }
        } else {
            count++;
            trees.pop();
        }
    }
    return count;
}

}