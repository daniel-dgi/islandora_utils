<?php

module_load_include('inc', 'islandora_utils', 'IslandoraUtilsPlugin');
class ValidateObjectPlugin extends IslandoraUtilsPlugin {

  /**
   * Constructor
   */
  function ValidateObjectPlugin($pid = NULL, $form_state = NULL, &$context = NULL) {
    parent::__construct($pid, $form_state, $context);
  }


  public function process() {
    module_load_include('inc', 'islandora', 'includes/IslandoraTuque');
    module_load_include('inc', 'islandora', 'includes/utilities');
    $datastreams = array();
    $object_models = array();
    if($this->fedora_object) {
      $object_models = $this->fedora_object->models;
      if(variable_get('islandora_utils_validate_object_settings_datastream_ids_merge', '')) {
        $content_model_datastreams = islandora_get_datastreams_requirements($this->fedora_object);
        foreach ($content_model_datastreams as $key => $value) {
          //dsm($key, 'cmodel dsid');
          $datastreams[$key] = $key;
        }
      }
      if(count($object_models) > 0) {
        $results = db_select('islandora_utils_plugin_validate_object_content_models', 'cmodel')
          ->fields('cmodel', array('content_model_id', 'content_model_name', 'content_model_datastreams'))
          ->condition('content_model_name', $object_models, 'IN')
          ->execute()
          ->fetchAllAssoc('content_model_id');

        foreach ($results as $cmodel) {
          $dsid_raw = explode(' ', $cmodel->content_model_datastreams);
          //dsm($dsid_raw, 'raw');
          foreach($dsid_raw as $dsid) {
            if(substr($dsid, 0, 1) == '-') {
              unset($datastreams[substr($dsid, 1)]);
            }
            else {
              if(!empty($dsid)) {
                $datastreams[$dsid] = $dsid;
              }
            }
          }
        }
      }

      //dsm($datastreams, 'dddddd');
      $object_datastreams = $this->fedora_object->repository->api->a->listDatastreams($this->fedora_object->id);

      foreach($datastreams as $dsid) {
        if(!isset($object_datastreams[$dsid])) {
          $this->results[] = array(
            'type' => 'error',
            'pid' => $this->fedora_object->id,
            'dsid' => $dsid,
            'message' => "Missing",
          );
        }
        else {
          $ds = $this->fedora_object[$dsid];
          if(variable_get('islandora_utils_validate_object_settings_datastream_size_zero', '') && $ds->size == -1) {
            $this->results[] = array(
              'type' => 'error',
              'pid' => $this->fedora_object->id,
              'dsid' => $dsid,
              'message' => "Size: Empty",
            );
          }
          else {
            $this->results[] = array(
              'type' => 'status',
              'pid' => $this->fedora_object->id,
              'dsid' => $dsid,
              'message' => "Size: $ds->size bytes",
            );
          }
        }
      }
    }
    else {
      $this->results[] = array(
        'type' => 'error',
        'pid' => $this->pid,
        'dsid' => 'n/a',
        'message' => "Object does not exist",
      );
    }

  }
}

