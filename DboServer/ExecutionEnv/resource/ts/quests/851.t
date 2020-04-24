CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 851;
	title = 85102;

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
				conv = 85109;
				ctype = 1;
				idx = 3032104;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 85108;
				gtype = 1;
				area = 85101;
				goal = 85104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 85105;
				stype = 2;
				taid = 1;
				title = 85102;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 85107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 29;
			}
			CDboTSClickNPC
			{
				npcidx = "3032104;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "852;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 85108;
			gtype = 1;
			oklnk = 2;
			area = 85101;
			goal = 85104;
			sort = 85105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 85102;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 85114;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 85101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1311205;";
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
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
	}
}

