CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 6;
	title = 602;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContUnifiedNarration
		{
			cancellnk = 1;
			cid = 3;
			idx = 61;
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

			CDboTSActNPCConv
			{
				conv = 609;
				ctype = 1;
				idx = 8031101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 608;
				gtype = 1;
				area = 601;
				goal = 604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 605;
				stype = 64;
				taid = 1;
				title = 602;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 607;
			nolnk = 253;
			rm = 0;
			yeslnk = 3;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 2;
			}
			CDboTSClickNPC
			{
				npcidx = "8031101;";
			}
			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSCheckPCCls
			{
				clsflg = 3;
			}
			CDboTSCheckClrQst
			{
				and = "5;";
				flink = 1;
				flinknextqid = "188;";
				not = 0;
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
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 608;
			gtype = 1;
			oklnk = 2;
			area = 601;
			goal = 604;
			sort = 605;
			prelnk = "3;3;";
			ds = 0;
			grade = 132203;
			rwd = 100;
			title = 602;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 614;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4181101;";
			}
		}
	}
}

