angular.module('codepuppy').controller('FileUploadCtrl', ['$scope', '$stateParams', '$fileUploader', function($scope, $stateParams, $fileUploader) {
    $scope.message = "I'm an upload page for the course " + $stateParams.classID;
    $scope.message+= " and assignment " + $stateParams.assignmentID;
}]);
