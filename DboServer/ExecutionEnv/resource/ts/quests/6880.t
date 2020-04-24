CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 6880;
	title = 688002;

	CNtlTSGroup
	{
		gid = 0;

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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 688007;
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
				minlvl = 15;
			}
			CDboTSClickNPC
			{
				npcidx = "4513104;";
			}
			CDboTSCheckClrQst
			{
				and = "6877;";
				flink = 0;
				flinknextqid = "6883;";
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 253;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 688009;
				ctype = 1;
				idx = 4513104;
				taid = 3;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 5811101;
				cnt0 = 10;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 2;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 688008;
				m1fx = "4312.890137";
				m2fz = "-1726.439941";
				m1fy = "0.000000";
				sort = 688005;
				m2widx = 1;
				grade = -1;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				gtype = 1;
				area = 688001;
				goal = 688004;
				m0fz = "-1856.969971";
				m0widx = 1;
				m1ttip = 688015;
				m2fy = "0.000000";
				m2pat = 2;
				scitem = -1;
				stype = 64;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-1922.390015";
				m2fx = "4296.450195";
				m2ttip = 688015;
				m0fx = "4495.970215";
				m0ttip = 688015;
				title = 688002;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;2;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 688008;
			gtype = 1;
			oklnk = 2;
			area = 688001;
			goal = 688004;
			sort = 688005;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 688002;
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 688014;
			nextlnk = 4;
			rwdexp = 0;
			rwdtbl = 688001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4513104;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "4;";
			type = 1;
		}
	}
}

