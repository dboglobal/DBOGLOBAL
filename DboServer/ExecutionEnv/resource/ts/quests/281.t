CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 281;
	title = 28102;

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
				conv = 28109;
				ctype = 1;
				idx = 6312101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 28108;
				gtype = 1;
				area = 28101;
				goal = 28104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 28105;
				stype = 1;
				taid = 1;
				title = 28102;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 28108;
			gtype = 1;
			oklnk = 2;
			area = 28101;
			goal = 28104;
			sort = 28105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 28102;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 28114;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 28101;
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
			stdiag = 28107;
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
				minlvl = 14;
			}
			CDboTSClickNPC
			{
				npcidx = "6312101;";
			}
			CDboTSCheckClrQst
			{
				and = "234;271;";
				flink = 1;
				flinknextqid = "292;";
				not = 0;
			}
		}
	}
}

