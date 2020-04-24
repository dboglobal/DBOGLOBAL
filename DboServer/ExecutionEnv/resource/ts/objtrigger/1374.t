CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 1374;
	title = 137402;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 254;
			prelnk = "5;";

			CDboTSActObjState
			{
				mstate = 0;
				oidx = -1;
				osh_sh = 1;
				widx = -1;
				osh_uc = 0;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 6;
			prelnk = "4;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 15000;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;2;";

			CDboTSActObjState
			{
				mstate = 0;
				oidx = -1;
				osh_sh = 0;
				widx = -1;
				osh_uc = 1;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "1;";

			CDboTSActTWaitTS
			{
				taid = 3;
				time = 1000;
			}
			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 137419;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 4;
			prelnk = "1;";

			CDboTSActTWaitTS
			{
				taid = 3;
				time = 1000;
			}
			CDboTSActPCConv
			{
				taid = 2;
				tblidx = 137418;
			}
			CDboTSActQItem
			{
				icnt0 = 1;
				iprob0 = "1.000000";
				iidx0 = 177;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContSwitch
		{
			bid1 = 1;
			cid = 1;
			cid1 = 2;
			prelnk = "13;";
			bid0 = 2;
			cid0 = 3;

			CDboTSActSWProbSF
			{
				fbi = 2;
				prob = "0.300000";
				sbi = 1;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 13;
			elnk = 255;
			nextlnk = 1;
			prelnk = "12;";

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
			cid = 12;
			prelnk = "11;";
			nolnk = 254;
			rm = 0;
			yeslnk = 13;

			CDboTSCheckOPObject
			{
			}
		}
		CDboTSContGAct
		{
			cid = 11;
			elnk = 255;
			nextlnk = 12;
			prelnk = "0;";

			CDboTSActOPObject
			{
				apply = 1;
				taid = 1;
				tblidx = 10003;
				time = 3000;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 137407;
			nolnk = 255;
			rm = 0;
			yeslnk = 11;

			CDboTSCheckObjItem
			{
				tid = 1374;
			}
			CDboTSClickObject
			{
				objidx = "697;698;699;700;701;702;";
				widx = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "6;12;";
			type = 1;
		}
	}
}

