<?php

/**
 * @file
 * Compound types.
 */

/**
 * All the types a compound can be.
 *
 * @return array
 *   array with name of compound as key and value a array with:
 *    - name: the human readable name of the type;
 *    - description: a description, optional;
 *    - predica.
 */
function islandora_compound_object_types() {
  return array(
    '' => array(
            'name' => t('Normal'),
          ),
    'hierarchical' => array(
            'name' => t('Hierarchical'),
          ),
  );
}

/**
 * Get the type of a compound object.
 */
function islandora_compound_object_get_type($object) {
  $rels = $object->relationships->get(ISLANDORA_RELS_EXT_URI, "isCompoundType");
  if (isset($rels[0]['object']['literal']) && $rels[0]['object']['literal'] && isset($rels[0]['object']['value'])) {
    return $rels[0]['object']['value'];
  }
  return '';
}

/**
 * Set the type of a compound object.
 */
function islandora_compound_object_set_type($object, $type) {
  $oldtype = islandora_compound_object_get_type($object);
  if ($oldtype != $type) {
    $object->relationships->remove(ISLANDORA_RELS_EXT_URI, "isCompoundType");
    $object->relationships->add(ISLANDORA_RELS_EXT_URI, "isCompoundType", $type, RELS_TYPE_PLAIN_LITERAL);
  }
}
