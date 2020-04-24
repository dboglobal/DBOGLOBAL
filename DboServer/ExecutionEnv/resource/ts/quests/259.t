CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 259;
	title = 25902;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 4;
			prelnk = "3;2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 3;

			CDboTSClickNPC
			{
				npcidx = "4751303;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContUsrSel
		{
			cancellnk = 4;
			cid = 3;
			lilnk = 255;
			ndesc0 = 25922;
			uspt = -1;
			desc = 25809;
			nid2 = 102;
			type = 0;
			ust = 0;
			idx = 4751303;
			ndesc2 = 25920;
			nid1 = 101;
			prelnk = "4;";
			ndesc1 = 25921;
			nid0 = 100;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 4;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 25909;
				ctype = 1;
				idx = 4751303;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 25908;
				gtype = 1;
				area = 25901;
				goal = 25904;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 25905;
				stype = 1;
				taid = 1;
				title = 25902;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 25907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCCls
			{
				clsflg = 16;
			}
			CDboTSCheckPCRace
			{
				raceflg = 2;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 10;
			}
			CDboTSClickNPC
			{
				npcidx = "4751303;";
			}
			CDboTSCheckClrQst
			{
				and = "257;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 102;
			rwdzeny = 0;
			desc = 25930;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 25903;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4751303;";
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 101;
			rwdzeny = 0;
			desc = 25931;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 25902;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4751303;";
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 25914;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 25901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4751303;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 25908;
			gtype = 1;
			oklnk = 2;
			area = 25901;
			goal = 25904;
			sort = 25905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 25902;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;101;102;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
	}
}

