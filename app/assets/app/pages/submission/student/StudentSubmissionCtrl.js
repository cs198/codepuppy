angular.module('codepuppy').controller('StudentSubmissionCtrl', ['$scope', '$routeParams', '$modal', '$http', '$location',
                                       function($scope, $routeParams, $modal, $http, $location) {

  var getFiles = function() {
    $http({method: 'GET', url: '/submissions/' + $routeParams.submissionID + '/submission_files.json'})
    .success(function(data, status, headers, config) {
      if(data.length > 0) {
        var url =  $location.$$path + '/files/' + data[0].id;
        $location.path(url);
      } else {
        $scope.message = "This submission appears to be empty.";
      }
    });
  };

  getFiles();

}]);