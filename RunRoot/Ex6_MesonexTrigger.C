{
  HipoChain chain;
  chain.Add("/path/to/data.hipo");

  //real data is missing banks, example:
  //(Warning , hipo::schema getEntryOrder(const char *name) item :superlayer not found, for bank  data for this item is not valid 

  //create histograms
  TH1F hmass("mesonmass","Invariant Mass of 2 pions - all events",200,0,2);
  TH1F hmass_trigger("mesonmass","Invariant Mass of 2 pions - with trigger",200,0,2);
    
  //create particles before looping to be more efficient
  TLorentzVector p4_pip;
  TLorentzVector p4_pim;

  //counters
  int nEvents = 0;
  int nTriggers = 0;
 
  //loop over files
  for(int ifile=0;ifile<chain.GetNFiles();++ifile){
    clas12reader c12{chain.GetFileName(ifile).Data()};

    //trigger class
    auto trig = mesonex_trigger(c12);
      
    //loop over all events in the file
    while(c12.next()==true){
      nEvents+=1;	    
      
      //count the number of events passing the trigger (before any topology)
      if(trig.fire()){
	nTriggers+=1;
      }

      //skip if no particles
      if(c12.getDetParticles().empty())
      	continue;
      
      //get the particles of interest
      auto parts=c12.getDetParticles();
      auto electrons=c12.getByID(11);
      auto gammas=c12.getByID(22);
      auto protons=c12.getByID(2212);
      auto pips=c12.getByID(211);
      auto pims=c12.getByID(-211);

      //assumptions for particular final state, skip otherwise
      if(!(electrons.size()>0 &&
	   protons.size()>0 &&
	   pips.size()>0 &&
	   pims.size()>0)){
	continue;
      }

      //get the pions
      auto pim = pims[0];
      auto pip = pips[0];
      
      //set the 4 vectors for the pions to make the meson
      p4_pip.SetXYZM(pip->par()->getPx(),pip->par()->getPy(),pip->par()->getPz(),0);
      p4_pim.SetXYZM(pim->par()->getPx(),pim->par()->getPy(),pim->par()->getPz(),0);
      auto meson = p4_pip + p4_pim;
      
      //histogram of meson mass from triggered events
      if(trig.fire()){
	hmass_trigger.Fill(meson.M());  //should be similar to the plot of all because of parts above
      }

      //histogram of meson mass for all events
      hmass.Fill(meson.M());
    }
  }
  
  //print some stats
  cout<<"Total number of events: "<<nEvents<<endl;
  cout<<"Total number of triggers: "<<nTriggers<<endl;
  cout<<"Ratio: "<<(float)nTriggers/nEvents<<endl;

  //draw the histogram
  TCanvas can;
  can.Divide(2,1);
  can.cd(1);
  hmass.Draw();
  can.cd(2);
  hmass_trigger.Draw();
  can.SaveAs("testing123.gif");
}
