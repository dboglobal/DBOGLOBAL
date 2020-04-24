CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2235;
	title = 223502;

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
				conv = 223509;
				ctype = 1;
				idx = 5062108;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 223508;
				gtype = 1;
				area = 223501;
				goal = 223504;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 223505;
				stype = 1;
				taid = 1;
				title = 223502;
			}
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
			desc = 223514;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 223501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5101302;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 223508;
			gtype = 1;
			oklnk = 2;
			area = 223501;
			goal = 223504;
			sort = 223505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 223502;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 223507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 51;
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "2236;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "5062108;";
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
	}
}

