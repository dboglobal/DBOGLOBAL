CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 1066;
	title = 106602;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "252;1;0;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 106607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 39;
			}
			CDboTSClickNPC
			{
				npcidx = "4511105;";
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "3;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 6;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActSToCEvt
			{
				dt2 = 106624;
				otype2 = 7;
				widx1 = 1;
				iidx0 = 788;
				otype1 = 7;
				widx2 = 1;
				dt0 = 106620;
				esctype = 0;
				iidx1 = 788;
				it1 = 106616;
				otype0 = 7;
				tidx2 = 637;
				dt1 = 106622;
				etype = 0;
				iidx2 = 788;
				it0 = 106615;
				taid = 1;
				tidx1 = 636;
				widx0 = 1;
				apptype = 0;
				eitype = 6;
				tidx0 = 635;
				ectype = -1;
				it2 = 106617;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 106614;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 106601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511105;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 106609;
				ctype = 1;
				idx = 4511105;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 106608;
				area = 106601;
				goal = 106604;
				m0fz = "309.000000";
				m0widx = 5;
				scitem = -1;
				sort = 106605;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "0.000000";
				m0ttip = 106626;
				rwd = 100;
				taid = 1;
				title = 106602;
				gtype = 1;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 106608;
			gtype = 1;
			oklnk = 2;
			area = 106601;
			goal = 106604;
			sort = 106605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 106602;
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 6;
				esctype = 0;
				ectype = -1;
				etype = 1;
				taid = 1;
			}
		}
	}
}

