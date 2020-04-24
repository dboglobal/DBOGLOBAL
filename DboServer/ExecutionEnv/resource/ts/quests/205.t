CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 205;
	title = 20502;

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
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 20509;
				ctype = 1;
				idx = 4651101;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 1413100;
				cnt0 = 4;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 20508;
				m1fx = "3411.229980";
				m1fy = "0.000000";
				sort = 20505;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-2442.610107";
				grade = 132203;
				m0fx = "3204.120117";
				m0ttip = 20515;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 20502;
				gtype = 2;
				area = 20501;
				goal = 20504;
				m0fz = "-2448.800049";
				m0widx = 1;
				m1ttip = 20515;
				scitem = -1;
				stype = 2;
			}
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 20508;
			gtype = 2;
			oklnk = 2;
			area = 20501;
			goal = 20504;
			sort = 20505;
			prelnk = "0;";
			ds = 0;
			grade = 132203;
			rwd = 100;
			title = 20502;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 20514;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 20501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511401;";
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
			stdiag = 20507;
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
				minlvl = 2;
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "212;";
				not = 0;
				or = "204;";
			}
			CDboTSClickNPC
			{
				npcidx = "4651101;";
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
				cnt1 = 0;
				eitype = 0;
				idx0 = 1413100;
				cnt0 = 5;
				cnt2 = 0;
				ectype = -1;
				etype = 1;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
		}
	}
}

