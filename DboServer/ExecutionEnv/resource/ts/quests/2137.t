CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2137;
	title = 213702;

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
				conv = 213709;
				ctype = 1;
				idx = 2801116;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 213708;
				gtype = 1;
				area = 213701;
				goal = 213704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 213705;
				stype = 1;
				taid = 1;
				title = 213702;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 213707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 51;
			}
			CDboTSClickNPC
			{
				npcidx = "2801116;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "2139;";
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
			cont = 213708;
			gtype = 1;
			oklnk = 2;
			area = 213701;
			goal = 213704;
			sort = 213705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 213702;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 213714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 213701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4153102;";
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

