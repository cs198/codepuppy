angular.module('codepuppy').controller('SubmissionFileSelectorCtrl',
['$scope', '$stateParams', '$fileUploader', '$http', '$modal', function($scope, $stateParams, $fileUploader, $http, $modal) {

  $scope.fileClicked = function(file) {
    $scope.$parent.$parent.selectedFile = file;
  };

}]);
