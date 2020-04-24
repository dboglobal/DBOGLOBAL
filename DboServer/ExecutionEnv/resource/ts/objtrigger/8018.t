CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 8018;
	title = 801802;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 106;
			elnk = 255;
			nextlnk = 51;
			prelnk = "41;";

			CDboTSActItem
			{
				iidx0 = 18064;
				stype0 = 1;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 105;
			elnk = 255;
			nextlnk = 51;
			prelnk = "41;";

			CDboTSActItem
			{
				iidx0 = 18034;
				stype0 = 1;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 104;
			elnk = 255;
			nextlnk = 51;
			prelnk = "31;";

			CDboTSActItem
			{
				iidx0 = 505008;
				stype0 = 1;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 103;
			elnk = 255;
			nextlnk = 51;
			prelnk = "31;";

			CDboTSActItem
			{
				iidx0 = 501008;
				stype0 = 1;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 102;
			elnk = 255;
			nextlnk = 51;
			prelnk = "21;";

			CDboTSActItem
			{
				iidx0 = 545008;
				stype0 = 1;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 51;
			prelnk = "21;";

			CDboTSActItem
			{
				iidx0 = 541008;
				stype0 = 1;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 52;
			elnk = 255;
			nextlnk = 255;
			prelnk = "51;";

			CDboTSActObjState
			{
				mstate = 0;
				oidx = 6;
				osh_sh = 0;
				widx = 300000;
				osh_uc = 1;
				taid = 1;
			}
		}
		CDboTSContSwitch
		{
			bid1 = 1;
			cid = 41;
			cid1 = 105;
			prelnk = "11;";
			bid0 = 2;
			cid0 = 106;

			CDboTSActSWProbSF
			{
				fbi = 2;
				prob = "0.500000";
				sbi = 1;
				taid = 1;
			}
		}
		CDboTSContSwitch
		{
			bid1 = 1;
			cid = 11;
			cid1 = 31;
			prelnk = "10;";
			bid0 = 2;
			cid0 = 41;

			CDboTSActSWProbSF
			{
				fbi = 2;
				prob = "0.250000";
				sbi = 1;
				taid = 1;
			}
		}
		CDboTSContSwitch
		{
			bid1 = 1;
			cid = 31;
			cid1 = 103;
			prelnk = "11;";
			bid0 = 2;
			cid0 = 104;

			CDboTSActSWProbSF
			{
				fbi = 2;
				prob = "0.500000";
				sbi = 1;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 51;
			elnk = 255;
			nextlnk = 52;
			prelnk = "106;105;104;103;102;101;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 2000;
			}
		}
		CDboTSContSwitch
		{
			bid1 = 1;
			cid = 21;
			cid1 = 101;
			prelnk = "10;";
			bid0 = 2;
			cid0 = 102;

			CDboTSActSWProbSF
			{
				fbi = 2;
				prob = "0.500000";
				sbi = 1;
				taid = 1;
			}
		}
		CDboTSContSwitch
		{
			bid1 = 1;
			cid = 10;
			cid1 = 21;
			prelnk = "3;";
			bid0 = 2;
			cid0 = 11;

			CDboTSActSWProbSF
			{
				fbi = 2;
				prob = "0.200000";
				sbi = 1;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 10;
			prelnk = "2;";

			CDboTSActOPObject
			{
				apply = 0;
				taid = 1;
				tblidx = -1;
				time = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 2;
			prelnk = "1;";
			nolnk = 255;
			rm = 0;
			yeslnk = 3;

			CDboTSCheckOPObject
			{
			}
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "0;";

			CDboTSActOPObject
			{
				apply = 1;
				taid = 1;
				tblidx = 10003;
				time = 3000;
			}
		}
		CDboTSContEnd
		{
			cid = 255;
			prelnk = "52;2;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 801807;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSClickObject
			{
				objidx = "6;";
				widx = 300000;
			}
		}
	}
}

