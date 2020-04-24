CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1213;
	title = 121302;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 1;
				idx0 = -1;
				cnt0 = 0;
				cnt2 = 0;
				ectype = -1;
				etype = 1;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
		}
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 121308;
			gtype = 2;
			oklnk = 2;
			area = 121301;
			goal = 121304;
			sort = 121305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 121302;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 121314;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 121301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSCheckSToCEvt
			{
				itype = 1;
			}
			CDboTSClickNPC
			{
				npcidx = "2801108;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 121307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 41;
			}
			CDboTSClickNPC
			{
				npcidx = "2801108;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 121309;
				ctype = 1;
				idx = 2801108;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 5;
				eitype = 1;
				idx0 = 158;
				cnt0 = 5;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 159;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 121308;
				m3widx = 1;
				m1fy = "0.000000";
				sort = 121305;
				m2widx = 1;
				grade = 132203;
				m1pat = 2;
				m1widx = 1;
				m3fx = "2026.000000";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				area = 121301;
				goal = 121304;
				m0fz = "3015.000000";
				m0widx = 1;
				m1ttip = 121316;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "3027.000000";
				m3ttip = 121316;
				scitem = -1;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "3015.000000";
				m2fx = "2026.000000";
				m2ttip = 121315;
				m0fx = "1722.000000";
				m0ttip = 121315;
				m3pat = 2;
				title = 121302;
				m1fx = "1722.000000";
				m2fz = "3027.000000";
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iidx1 = 159;
				iprob0 = "1.000000";
				icnt1 = 255;
				iidx0 = 158;
				iprob1 = "1.000000";
				taid = 1;
				type = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 121307;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}

