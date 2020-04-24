CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 7;
	rq = 0;
	tid = 2285;
	title = 228502;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 5;
			prelnk = "4;";
			nolnk = 255;
			rm = 0;
			yeslnk = 6;

			CDboTSRcvSvrEvt
			{
				id = 495;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 100;
			prelnk = "5;";

			CDboTSActWorldPlayScript
			{
				sid = 2285;
				start = 0;
				taid = 1;
				uof = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 252;
			nextlnk = 5;
			prelnk = "3;";

			CDboTSActWorldPlayScript
			{
				sid = 2285;
				start = 1;
				taid = 1;
				uof = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 3000;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 228507;
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
				npcidx = "7632105;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "2286;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 228508;
			gtype = 2;
			oklnk = 2;
			area = 228501;
			goal = 228504;
			sort = 228505;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 228502;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 228514;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 228501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "6;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "7133104;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "4;2;";
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
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
				conv = 228509;
				ctype = 1;
				idx = 7632105;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 228508;
				gtype = 2;
				area = 228501;
				goal = 228504;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 228505;
				stype = 1;
				taid = 1;
				title = 228502;
			}
		}
	}
}

