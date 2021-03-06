<?php

/**
 * @file
 * theme/theme.inc
 *
 * Theme functions.
 */

/**
 * Theme function for the compound navigation block parts.
 */
function template_preprocess_islandora_compound_prev_next(array &$variables) {
  drupal_add_js(drupal_get_path('module', 'islandora_compound_object') . '/js/caption_width.js');
  drupal_add_js(drupal_get_path('module', 'islandora_compound_object') . '/js/show_selected.js');

  $themed_siblings = array();
  $image_path = drupal_get_path('module', 'islandora');
  $folder_image_path = "$image_path/images/folder.png";

  // If the parent hasModel islandora:compoundCModel,
  // the first child is typically identical to the parent;
  // do not show the parent TN.

  // If the parent does not have hasModel islandora:compoundCModel,
  // the first child and parent are different;
  // show the parent TN.

  $variables['parent_tn'] = NULL;
  $parent_object = islandora_object_load($variables['parent_pid']);
  if ($parent_object && !in_array(ISLANDORA_COMPOUND_OBJECT_CMODEL, $parent_object->models)) {
    if (isset($parent_object['TN']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $parent_object['TN'])) {
      $variables['parent_tn'] = 'islandora/object/' . $parent_object->id . '/datastream/TN/view';
    }
    else {
      $variables['parent_tn'] = $folder_image_path;
    }
  }

  module_load_include('inc', 'islandora_compound_object', 'includes/compound_types');
  $compoundtype = islandora_compound_object_get_type($parent_object);
  $objecttypes = islandora_compound_object_types();

  if (isset($objecttypes[$compoundtype]['javascript'])) {
    drupal_add_js(drupal_get_path('module', 'islandora_compound_object') . '/js/' . $objecttypes[$compoundtype]['javascript']);
  }

  // If the parent TN is not set, only show children if there's more than one.
  // If the parent TN is set, show all children even if there's only one.

  if (($variables['child_count'] > 1 && !$variables['parent_tn']) || ($variables['child_count'] > 0 && $variables['parent_tn'])) {
    $parent_label = $parent_object->label;
    for ($i = 0; $i < count($variables['siblings']); $i += 1) {
      $sibling = array();
      $sibling['pid'] = $variables['siblings'][$i];
      $sibling['class'] = array();
      if ($sibling['pid'] === $variables['pid']) {
        $sibling['class'][] = 'active';
      }
      $sibling_object = islandora_object_load($sibling['pid']);
      if (isset($sibling_object['TN']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $sibling_object['TN'])) {
        $sibling['TN'] = 'islandora/object/' . $sibling['pid'] . '/datastream/TN/view';
      }
      else {
        // Object either does not have a thumbnail or it's restricted show a
        // default in its place.
        $sibling['TN'] = $folder_image_path;
      }
      $sibling['label'] = $sibling_object->label;

      $label = preg_replace('!^\s*' . preg_quote($parent_label, '!') . '[ ,.;:/-]+!', "", $sibling['label']);
      if ($label === $sibling['label']) {
        if (preg_match('/^(.*)\.\.\.[ ,.;:\/-]+(.*)$/', $label, $matches)) {
          $partofparent = $matches[1];
          if (substr($parent_label, 0, strlen($partofparent)) === $partofparent) {
            $label = $matches[2];
          }
        }
      }
      if ($label === NULL || strlen($label) ==  0 || ctype_space($label)) {
        $label = $sibling['label'];
      }
      $sibling['shortlabel'] = $label;

      if (isset($objecttypes[$compoundtype]['relations']['predicate'])) {
        $value = islandora_compound_object_get_relation($sibling_object, $objecttypes[$compoundtype]['relations']['predicate'], $parent_object);
        $sibling['relation'] = $value;
      }
      $themed_siblings[] = $sibling;
    }
  }
  if (!empty($compoundtype)) {
    $variables['theme_hook_suggestions'] += theme_get_suggestions(array($compoundtype), 'islandora_compound_prev_next');
  }
  $variables['themed_siblings'] = $themed_siblings;
}

/**
 * Implements theme_element().
 */
function theme_islandora_compound_object_draggable_table(array $variables) {
  $form = $variables['form'];
  $rows = array();
  $weight_class = 'islandora-page-weight';
  $table = &$form['table'];
  $headerkeys = array_keys($table['#header']);
  foreach (element_children($table['rows']) as $id) {
    $row = &$table['rows'][$id];
    $row['weight']['#attributes'] = array('class' => array($weight_class));
    $data = array();
    foreach ($headerkeys as $headerkey) {
      $data[$headerkey] = drupal_render($row[$headerkey]);
    }
    $rows[] = array(
      'data' => $data,
      'class' => array('draggable'),
    );
  }
  $table_id = 'islandora-reorder-objects-table';
  $output = theme('table', array(
              'header' => $table['#header'],
              'rows' => $rows,
              'attributes' => array('id' => $table_id)));
  $output .= drupal_render_children($form);
  drupal_add_tabledrag($table_id, 'order', 'sibling', $weight_class);
  return $output;
}

/**
 * Implements hook_preprocess_theme().
 */
function template_preprocess_islandora_compound_object(array &$variables) {
  module_load_include('inc', 'islandora', 'includes/metadata');
  drupal_add_js('misc/form.js');
  drupal_add_js('misc/collapse.js');
  $islandora_object = $variables['islandora_object'];

  // Thumbnail.
  if (isset($islandora_object['TN']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $islandora_object['TN'])) {
    $thumbnail_size_url = url("islandora/object/{$islandora_object->id}/datastream/TN/view");
    $params = array(
      'title' => $islandora_object->label,
      'path' => $thumbnail_size_url,
    );
    $variables['islandora_thumbnail_img'] = theme('image', $params);
  }
  $variables['parent_collections'] = islandora_get_parents_from_rels_ext($islandora_object);
  $variables['metadata'] = islandora_retrieve_metadata_markup($islandora_object);
  $variables['description'] = islandora_retrieve_description_markup($islandora_object);
}

