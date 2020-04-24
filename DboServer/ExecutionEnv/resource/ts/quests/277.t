CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 277;
	title = 27702;

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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 27714;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 27701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511503;";
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
			stdiag = 27707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 2;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 12;
			}
			CDboTSClickNPC
			{
				npcidx = "4511503;";
			}
			CDboTSCheckClrQst
			{
				and = "275;";
				flink = 0;
				not = 0;
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
				conv = 27709;
				ctype = 1;
				idx = 4511503;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 1;
				eitype = 0;
				idx0 = 4511107;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 3212109;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 27708;
				m1fx = "-549.190002";
				m2fz = "-3444.000000";
				m1fy = "0.000000";
				sort = 27705;
				m2widx = 1;
				grade = 132203;
				m1pat = 1;
				m1widx = 6;
				rwd = 100;
				taid = 1;
				gtype = 3;
				area = 27701;
				goal = 27704;
				m0fz = "1184.229980";
				m0widx = 6;
				m1ttip = 27716;
				m2fy = "0.000000";
				m2pat = 0;
				scitem = -1;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 1;
				m1fz = "1048.819946";
				m2fx = "4361.000000";
				m2ttip = 27717;
				m0fx = "-420.200012";
				m0ttip = 27715;
				title = 27702;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 27708;
			gtype = 3;
			oklnk = 2;
			area = 27701;
			goal = 27704;
			sort = 27705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 27702;
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 0;
			}
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
				cnt1 = 1;
				eitype = 0;
				idx0 = 4511107;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 1;
				idx1 = 3212109;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
		}
	}
}

