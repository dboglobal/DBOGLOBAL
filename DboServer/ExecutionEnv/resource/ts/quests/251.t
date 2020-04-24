CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 251;
	title = 25102;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContUnifiedNarration
		{
			cancellnk = 1;
			cid = 3;
			idx = 2511;
			lilnk = 255;
			oklnk = 1;
			prelnk = "0;";
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActRegQInfo
			{
				cont = 25108;
				gtype = 1;
				area = 25101;
				goal = 25104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 25105;
				stype = 2;
				taid = 1;
				title = 25102;
			}
			CDboTSActNPCConv
			{
				conv = 25109;
				ctype = 1;
				idx = 4651102;
				taid = 2;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 25108;
			gtype = 1;
			oklnk = 2;
			area = 25101;
			goal = 25104;
			sort = 25105;
			prelnk = "3;3;";
			ds = 0;
			grade = 132203;
			rwd = 100;
			title = 25102;
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 25114;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 25101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4751201;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 25107;
			nolnk = 253;
			rm = 0;
			yeslnk = 3;

			CDboTSCheckPCRace
			{
				raceflg = 2;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 9;
			}
			CDboTSClickNPC
			{
				npcidx = "4651102;";
			}
			CDboTSCheckClrQst
			{
				and = "249;";
				flink = 1;
				flinknextqid = "254;";
				not = 0;
			}
		}
	}
}

