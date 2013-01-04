<?php

module_load_include('inc', 'islandora_utils', 'IslandoraUtilsPlugin');
class ValidateObjectPlugin extends IslandoraUtilsPlugin {

  /**
   * Constructor
   */
  function ValidateObjectPlugin($pid = NULL, $form_state = NULL) {
    parent::__construct($pid, $form_state);
    }


  public function process() {
    module_load_include('inc', 'islandora', 'includes/IslandoraTuque');
    module_load_include('inc', 'islandora', 'includes/utilities');


    $models_object = $this->fedora_object->models;
    $datastreams_object = $this->fedora_object->repository->api->a->listDatastreams($this->fedora_object->id);
    $datastreams_object_content_model = islandora_get_datastreams_requirements($this->fedora_object);

    $models_raw = variable_get('islandora_utils_validate_object_settings_content_models', array());
    $models = explode(' ', $models_raw);

    $datastreams_raw = variable_get('islandora_utils_validate_object_settings_datastream_ids', ' ');
    $datastreams = explode(' ', $datastreams_raw);
    $datastreams = array_flip($datastreams);

    if(variable_get('islandora_utils_validate_object_settings_datastream_ids_merge', '')) {
      $datastreams = array_merge($datastreams, $datastreams_object_content_model);
    }

    $rels = $this->fedora_object->relationships->get();

    /*
    dsm($models,'models');
    dsm($models_object,'models object');
    dsm($rels,'rels');
    dsm($datastreams,'ds');
    dsm($datastreams_object_content_model,'ds from comdel');
    dsm($fedora_object,'obj');
    */


    foreach($models as $model) {
      if(in_array($model, $models_object)) {
        foreach($datastreams as $datastream => $value) {
          if($ds = $this->fedora_object[$datastream]) {
            if(variable_get('islandora_utils_validate_object_settings_datastream_size_zero', '')) {
              if($ds->size == -1) {
                $this->results[] = array(
                  'type' => 'error',
                  'pid' => $this->fedora_object->id,
                  'message' => "Datastream ($datastream) is 0 bytes",
                );
              }
              else {
                $this->results[] = array(
                  'type' => 'status',
                  'pid' => $this->fedora_object->id,
                  'message' => "Datastream ($datastream) is $ds->size bytes",
                );
              }
            }
          }
          else {
            $this->results[] = array(
              'type' => 'error',
              'pid' => $this->fedora_object->id,
              'message' => "Datastream ($datastream) is missing",
            );
          }
        }
      }
    }
  }
}

