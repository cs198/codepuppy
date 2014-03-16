angular.module('codepuppy').controller('StudentAssignmentCtrl', ['$scope',
    '$routeParams', '$fileUploader', '$http',
    function($scope, $routeParams, $fileUploader, $http) {

  var getAssignment = function() {
    $http({
      method: 'GET',
      url: '/assignments/' + $routeParams.assignmentID + '.json'
    })
    .success(function(data, status, headers, config) {
      $scope.assignmentDescription = data.description;
      $scope.assignmentName = data.name;

      if (!$scope.assignmentDescription) {
        $scope.assignmentDescription = '';
      }

      if (!$scope.assignmentName) {
        $scope.assignmentName = '';
      }
    });
  };
  getAssignment();

  $scope.message = 'I\'m an assignment page for the course ' +
      $routeParams.courseID;
  $scope.message += ' and assignment ' + $routeParams.assignmentID;
  $scope.uploadPath = '/#/' + $routeParams.courseID + '/' +
      $routeParams.assignmentID + '/submit';
}]);
