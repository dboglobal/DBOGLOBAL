CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 781;
	title = 78102;

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
				conv = 78109;
				ctype = 1;
				idx = 3032102;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 78108;
				gtype = 1;
				area = 78101;
				goal = 78104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 78105;
				stype = 1;
				taid = 1;
				title = 78102;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 78107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 26;
			}
			CDboTSClickNPC
			{
				npcidx = "3032102;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "782;";
				not = 0;
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 78108;
			gtype = 1;
			oklnk = 2;
			area = 78101;
			goal = 78104;
			sort = 78105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 78102;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 78114;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 78101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3031201;";
			}
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

