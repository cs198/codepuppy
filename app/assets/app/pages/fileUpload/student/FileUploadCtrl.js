angular.module('codepuppy').controller('FileUploadCtrl', ['$scope', '$routeParams', '$fileUploader', function($scope, $routeParams, $fileUploader) {
    $scope.message = "I'm an upload page for the course " + $routeParams.classID;
    $scope.message+= " and assignment " + $routeParams.assignmentID;
}]);