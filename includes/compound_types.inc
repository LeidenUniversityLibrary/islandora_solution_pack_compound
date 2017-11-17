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
      'relations' => array(
        'level' => array(
          'title' => t('Level'),
          'predicate' => 'isHierarchicalLevelOf',
          'input' => array(
            'validation' => array(
              'required' => TRUE,
              'pattern' => '[1-9][0-9]*(\.[1-9][0-9]*)*', 
              'title' => t('Numbers separated by points, e.g. 4 or 1.2 or 3.10.5'),
            ),
          ),
        ),
      ),
      'comparefunction' => 'islandora_compound_object_hierarchical_compare',
      'javascript' => 'hierarchical.js',
    ),
  );
}

/**
 * Get the type of a compound object.
 */
function islandora_compound_object_get_type($object) {
  return islandora_compound_object_get_by_predicate($object, 'isCompoundType');
}

/**
 * Get the specific relation of an object.
 */
function islandora_compound_object_get_relation($object, $relation, $toobject) {
  $escaped_pid = str_replace(':', '_', $toobject->id);
  return islandora_compound_object_get_by_predicate($object, $relation . $escaped_pid);
}

/**
 * Get the value of a relation by a specific predicate.
 */
function islandora_compound_object_get_by_predicate($object, $predicate) {
  $rels = $object->relationships->get(ISLANDORA_RELS_EXT_URI, $predicate);
  if (isset($rels[0]['object']['literal']) && $rels[0]['object']['literal'] && isset($rels[0]['object']['value'])) {
    return $rels[0]['object']['value'];
  }
  return '';
}

/**
 * Set the type of a compound object.
 */
function islandora_compound_object_set_type($object, $type) {
  return islandora_compound_object_set_by_predicate($object, 'isCompoundType', $type);
}

/**
 * Set the specific relation of an object.
 */
function islandora_compound_object_set_relation($object, $relation, $toobject, $value) {
  $escaped_pid = str_replace(':', '_', $toobject->id);
  return islandora_compound_object_set_by_predicate($object, $relation . $escaped_pid, $value);
}

/**
 * Set the value of a relation by a specific predicate.
 */
function islandora_compound_object_set_by_predicate($object, $predicate, $value) {
  $oldvalue = islandora_compound_object_get_by_predicate($object, $predicate);
  if ($oldvalue != $value) {
    $object->relationships->autoCommit = FALSE;
    $object->relationships->remove(ISLANDORA_RELS_EXT_URI, $predicate);
    $object->relationships->add(ISLANDORA_RELS_EXT_URI, $predicate, $value, RELS_TYPE_PLAIN_LITERAL);
    $object->relationships->commitRelationships();
  }
}

/**
 * Compare 2 hierarchical level values
 */
function islandora_compound_object_hierarchical_compare($a, $b) {
  $la = explode('.', $a['level']);
  $lb = explode('.', $b['level']);
  
  $m = min(count($la), count($lb));
  for ($i = 0; $i < $m; $i++) {
    $na = (integer)$la[$i];
    $nb = (integer)$lb[$i];
    if ($na > $nb) {
      return 1;
    }
    if ($na < $nb) {
      return -1;
    }
  }
  return strcmp($a['level'], $b['level']);
}
