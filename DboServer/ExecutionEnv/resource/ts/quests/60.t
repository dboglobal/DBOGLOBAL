CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 60;
	title = 6002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 6009;
				ctype = 1;
				idx = 3331201;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 6008;
				gtype = 1;
				area = 6001;
				goal = 6004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 6005;
				stype = 1;
				taid = 1;
				title = 6002;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 6007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCCls
			{
				clsflg = 2;
			}
			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 10;
			}
			CDboTSClickNPC
			{
				npcidx = "3331201;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "63;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 6008;
			gtype = 1;
			oklnk = 2;
			area = 6001;
			goal = 6004;
			sort = 6005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 6002;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 6014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 6001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3331101;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
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

